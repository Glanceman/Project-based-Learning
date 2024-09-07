#pragma once

#include "net_message.h"
#include "net_server.h"
#include "net_tsqueue.h"
#include <cstring>
#include <iostream>
#include <locale.h>
#include <memory>
#include <string>
#include <system_error>
#include <utility>
#include <winnt.h>
#include <cstdint>
#include <openssl/evp.h>
#include <sstream>
#include <asio/steady_timer.hpp>

namespace Olc
{

    namespace Net
    {
        template <typename T>
        class Server_Interface;

        inline std::string HexToString(const unsigned char arr[], unsigned int len)
        {
            std::stringstream ss;
            ss << std::hex << std::setfill('0');
            for (unsigned int i = 0; i < len; ++i)
            {
                ss << std::setw(2) << static_cast<int>(arr[i]);
            }

            return ss.str();
        }

        ///@brief encrypt the data
        inline std::unique_ptr<unsigned char[]> Scramble(std::string &seed)
        {
            EVP_MD_CTX   *context = EVP_MD_CTX_new();
            const EVP_MD *md      = EVP_md5();

            std::unique_ptr<unsigned char[]> digest = std::make_unique<unsigned char[]>(EVP_MAX_MD_SIZE);
            // unsigned char digest[EVP_MAX_MD_SIZE];
            unsigned int digestLen;

            EVP_DigestInit_ex2(context, md, NULL);
            EVP_DigestUpdate(context, seed.c_str(), seed.length());
            EVP_DigestFinal_ex(context, digest.get(), &digestLen);
            EVP_MD_CTX_free(context);

            return digest;
        }

        template <typename T>
        class Connection : public std::enable_shared_from_this<Connection<T>>
        {
        public:
            // A connection is "owned" by either a server or a client, and its
            // behaviour is slightly different bewteen the two.
            enum class OwnerType
            {
                Server,
                Client
            };
            // Constructor: Specify Owner, connect to context, transfer the socket
            // Provide reference to incoming message queue
            Connection(OwnerType ownerType, asio::io_context &asioContext, asio::ip::tcp::socket socket, Tsqueue<OwnedMessage<T>> &qIn) :
                _asioContext(asioContext), _socket(std::move(socket)), _messageQueueIn(qIn)
            {
                _ownerType = ownerType;

                if (_ownerType == OwnerType::Server)
                {
                    _nonce           = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
                    std::string seed = std::to_string(_nonce);
                    // convert hex to string
                    std::unique_ptr<unsigned char[]> digest = Scramble(seed);
                    _handShakeCheck                         = HexToString(digest.get(), MD5Length);

                    _timer = std::make_unique<asio::steady_timer>(_asioContext, std::chrono::seconds(_timeInterval));
                }
            }

            virtual ~Connection()
            {
            }

        public:
            /// @brief client connect to the server
            /// @param endpoints
            void ConnectToServer(const asio::ip::tcp::resolver::results_type &endpoints)
            {
                if (_ownerType == OwnerType::Client)
                {
                    //_socket.connect(endpoints[0],error_code);
                    asio::async_connect(_socket, endpoints, [this](std::error_code error_code, asio::ip::tcp::endpoint endpoint) {
                        if (error_code)
                        {
                            std::cout << "\nConnection failure" << error_code.message();
                            return;
                        }
                        // handshake and continue
                        //  send out the computed challenge
                        ReadValidationClient();
                    });
                }
            }

            /// @brief record the id of the connection to client and listen the message (Server)
            /// @param id
            void ConnectToClient(Server_Interface<T> *server, uint32_t id)
            {
                if (_ownerType == OwnerType::Server)
                {
                    if (_socket.is_open())
                    {
                        _id = id;

                        // Start Handshake
                        //  send challenge
                        WriteValidationServer();
                        _timer->async_wait([this](std::error_code ec) {
                            if (!ec)
                            {
                                std::cout << "\nCalled From Timer";
                                if (_handShakeSuccess == false)
                                {
                                    _socket.close();
                                    std::cout << "\nTimeout";
                                }
                            }
                            else
                            {
                                std::cout << "\nError from Timer";
                            }
                        });
                        // validated the receive answer
                        ReadValidationServer(server);
                    }
                }
            }

            void Disconnect()
            {
                if (IsConnected())
                {
                    asio::post(_asioContext,
                               [this]() {
                                   _socket.close();
                               });
                }
            };

            bool IsConnected() const
            {
                return _socket.is_open();
            };
            void Send(const Message<T> &msg)
            {
                asio::post(_asioContext,
                           [this, msg]() {
                               // If the queue has a message in it, then we must
                               // assume that it is in the process of asynchronously being written.
                               // Either way add the message to the queue to be output. If no messages
                               // were available to be written, then start the process of writing the
                               // message at the front of the queue.
                               bool bWritingMessage = !_messageQueueOut.empty();
                               _messageQueueOut.emplace_back(msg);
                               if (bWritingMessage == false)
                               {
                                   WriteHeader();
                               }
                           });
            }

            // This ID is used system wide - its how clients will understand other clients
            // exist across the whole system.
            uint32_t GetID() const
            {
                return _id;
            }

        private:
            /// @brief Async context ready to read a message header
            void ReadHeader()
            {
                asio::async_read(_socket, asio::buffer(&_incomingMessageTemp.header, sizeof(Message_header<T>)), [this](std::error_code ec, std::size_t length) {
                    if (ec)
                    {
                        std::cout << "\n[ " << _id << " ] " << "Read Header Fail";
                        _socket.close();
                        return;
                    }

                    if (_incomingMessageTemp.header.size > 0) // has body
                    {
                        _incomingMessageTemp.body.resize(_incomingMessageTemp.header.size);
                        ReadBody();
                    }
                    else
                    {
                        AddToIncomingMessageQueue();
                    }
                });
            }
            /// @brief Async context ready to read a message body
            void ReadBody()
            {
                asio::async_read(_socket, asio::buffer(_incomingMessageTemp.body.data(), _incomingMessageTemp.body.size()), [this](std::error_code ec, std::size_t length) {
                    if (ec)
                    {
                        std::cout << "\n[ " << _id << " ] " << "Read Body Fail";
                        _socket.close();
                        return;
                    }
                    AddToIncomingMessageQueue();
                });
            }
            /// @brief Async
            void WriteHeader()
            {
                asio::async_write(_socket, asio::buffer(&_messageQueueOut.front().header, sizeof(Message_header<T>)), [this](std::error_code ec, std::size_t length) {
                    if (ec)
                    {
                        std::cout << "\n[ " << _id << " ] " << "Write Header Fail";
                        _socket.close();
                        return;
                    }

                    if (_messageQueueOut.front().body.size() > 0)
                    { // check whether the packet has body
                        WriteBody();
                    }
                    else
                    {                                          // no body
                        _messageQueueOut.pop_front();          // remove sent header;
                        if (_messageQueueOut.empty() == false) // still have message
                        {
                            WriteHeader();
                        }
                    }
                });
            }
            /// @brief Async
            void WriteBody()
            {
                // write the body into the socket which is sending
                asio::async_write(_socket, asio::buffer(_messageQueueOut.front().body.data(), _messageQueueOut.front().body.size()), [this](std::error_code ec, std::size_t length) {
                    if (ec)
                    {
                        std::cout << "\n[ " << _id << " ] " << "Write body Fail";
                        _socket.close();
                        return;
                    }

                    _messageQueueOut.pop_front();
                    if (_messageQueueOut.empty() == false) // still have message
                    {                                      // data are writing to send
                        WriteHeader();
                    }
                });
            }

            /// @brief  push incoming message to incomming message queue
            void AddToIncomingMessageQueue()
            {
                switch (_ownerType)
                {
                case OwnerType::Server /* constant-expression */: {
                    /* code */
                    _messageQueueIn.emplace_back({this->shared_from_this() /*remote*/, _incomingMessageTemp /*msg */});
                }
                break;
                case OwnerType::Client: {
                    _messageQueueIn.emplace_back({nullptr /*remote*/, _incomingMessageTemp /*msg */});
                }
                break;
                default:
                    break;
                }

                ReadHeader();
            }

            /// Begin Handshake code
            ///@brief send the nonce
            void WriteValidationServer()
            {
                asio::async_write(_socket, asio::buffer(&_nonce, sizeof(uint64_t)), [this](std::error_code ec, std::size_t length) {
                    if (ec)
                    {
                        std::cout << "\n[ " << _id << " ] " << "Failure: Send Challenge To Client";
                        _socket.close();
                        return;
                    }
                });
            }

            void ReadValidationServer(Server_Interface<T> *server = nullptr)
            {
                asio::async_read(_socket, asio::buffer(_digest, EVP_MAX_MD_SIZE), [this, server](std::error_code ec, std::size_t length) {
                    if (!ec)
                    {
                        if (_ownerType != OwnerType::Server) { return; }
                        // convert hex digest to string
                        std::string digest = HexToString(_digest, MD5Length);
                        std::cout << "\n Digest = " << digest;
                        if (digest != _handShakeCheck)
                        {
                            std::cout << "\nFailure: Handshake";
                            _socket.close();
                        }

                        std::cout << "\nSuccessful: Handshake";
                        _handShakeSuccess = true;
                        server->OnCilentValidated(this->shared_from_this());
                        ReadHeader();
                    }
                    else
                    {
                        std::cout << "\nFailure: Handshake ( Read Validation )";
                        _socket.close();
                    }
                });
            }

            /// @brief used to receive nonce
            void ReadValidationClient()
            {
                asio::async_read(_socket, asio::buffer(&_nonce, sizeof(uint64_t)), [this](std::error_code ec, std::size_t length) {
                    if (!ec)
                    {
                        if (_ownerType != OwnerType::Client) { return; }
                        uint64_t                         nonce  = _nonce;
                        std::string                      seed   = std::to_string(nonce);
                        std::unique_ptr<unsigned char[]> digest = Scramble(seed);
                        // copy the content
                        std::memcpy(_digest, digest.get(), EVP_MAX_MD_SIZE);
                        // send out the digest
                        WriteValidationClient();
                    }
                    else
                    {
                        std::cout << "\nFailure: Handshake - Receiving Challenge";
                        _socket.close();
                    }
                });
            }

            void WriteValidationClient()
            {
                // send the digest async
                asio::async_write(_socket, asio::buffer(_digest, EVP_MAX_MD_SIZE), [this](std::error_code ec, std::size_t length) {
                    if (ec)
                    {
                        std::cout << "\n[ " << _id << " ] " << "Failure: Send Proof";
                        _socket.close();
                        return;
                    }
                    else
                    {
                        std::cout << "\nSuccessful: Handshake";
                        // listen the following message
                        ReadHeader();
                    }
                });
            }
            /// End Handshake code
        protected:
            // each connection has its own socket
            asio::ip::tcp::socket _socket;
            // shared
            asio::io_context &_asioContext;

            // hold message and sent them out
            Tsqueue<Message<T>> _messageQueueOut;

            Message<T> _incomingMessageTemp;
            // hold all recevied message
            Tsqueue<OwnedMessage<T>> &_messageQueueIn;

            OwnerType _ownerType = OwnerType::Server;

            uint32_t _id = 0;

            // handshake
            unsigned char _digest[EVP_MAX_MD_SIZE];
            uint64_t      _nonce            = 0;
            std::string   _handShakeCheck   = "";
            bool          _handShakeSuccess = false;
            const int     MD5Length         = 16; // bytes

            std::unique_ptr<asio::steady_timer> _timer;
            const int                           _timeInterval = 1;
        };
    } // namespace Net

} // namespace Olc