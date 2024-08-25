#pragma once

#include "net_message.h"
#include "net_tsqueue.h"
#include "net_common.h"
#include <memory>

namespace Olc
{

    namespace Net
    {
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
            Connection(OwnerType ownerType, asio::io_context &asioContext, asio::ip::tcp::socket socket, Tsqueue<OwnedMessage<T>> &qIn)
                : _asioContext(asioContext), _socket(std::move(socket)), _messageQueueIn(qIn)
            {
                _ownerType = ownerType;
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
                    asio::async_connect(_socket, endpoints,
                                        [this](std::error_code error_code, asio::ip::tcp::endpoint endpoint)
                                        {
                                            if (error_code)
                                            {
                                                std::cout << "\nConnection failure" << error_code.message();
                                                return;
                                            }
                                            ReadHeader();
                                        });
                }
            }

            // @brief record the id of the connection to client and listen the message
            /// @param id
            void ConnectToClient(uint32_t id)
            {
                if (_ownerType == OwnerType::Server)
                {
                    if (_socket.is_open())
                    {
                        _id = id;
                        // wait for the client message
                        ReadHeader();
                    }
                }
            }

            void Disconnect()
            {
                if (IsConnected())
                {
                    asio::post(_asioContext,
                               [this]()
                               {
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
                           [this, msg]()
                           {
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
                asio::async_read(_socket, asio::buffer(&_incomingMessageTemp.header, sizeof(Message_header<T>)),
                                 [this](std::error_code ec, std::size_t length)
                                 {
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
                asio::async_read(_socket, asio::buffer(_incomingMessageTemp.body.data(), _incomingMessageTemp.body.size()),
                                 [this](std::error_code ec, std::size_t length)
                                 {
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
                asio::async_write(_socket, asio::buffer(&_messageQueueOut.front().header, sizeof(Message_header<T>)),
                                  [this](std::error_code ec, std::size_t length)
                                  {
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
                asio::async_write(_socket, asio::buffer(_messageQueueOut.front().body.data(), _messageQueueOut.front().body.size()),
                                  [this](std::error_code ec, std::size_t length)
                                  {
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
                case OwnerType::Server /* constant-expression */:
                    {
                        /* code */
                        _messageQueueIn.emplace_back({this->shared_from_this() /*remote*/, _incomingMessageTemp /*msg */});
                    }
                    break;
                case OwnerType::Client:
                {

                    _messageQueueIn.emplace_back({nullptr /*remote*/, _incomingMessageTemp /*msg */});
                }
                break;
                default:
                    break;
                }

                ReadHeader();
            }

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
        };
    }

}