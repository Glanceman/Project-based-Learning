#pragma once

#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace Olc
{

    namespace Net
    {
        template <typename T>
        class Server_Interface
        {
        public:
            Server_Interface(uint16_t portNumber) :
                _portNumber(portNumber), _acceptor(_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), portNumber))
            {
            }

            virtual ~Server_Interface()
            {
                Stop();
            }

            bool Start()
            {
                std::cout << "\n[Server] Starting ";
                try
                {
                    _threadContext = std::thread([this]() { _context.run(); });
                    WaitForClientConnection();
                }
                catch (std::exception exception)
                {
                    std::cout << "\n[Server] Exception: " << exception.what();
                    return false;
                }
                std::cout << "\n[Server] Listeninng on " << _portNumber;
                std::cout << "\n[Server] Started ";
                return true;
            }

            void Stop()
            {
                std::cout << "\n[Server] Stopping ";
                _context.stop(); // request the context to close
                if (_threadContext.joinable())
                { // tidy up the context thread
                    _threadContext.join();
                }
                std::cout << "\n[Server] Stopped ";
            }

            /// @brief Async - Instruct asio to wait for connection
            void WaitForClientConnection()
            {
                std::cout << "\n[Server] Waiting for Client to connect ";
                _acceptor.async_accept(
                    [this](std::error_code error_code, asio::ip::tcp::socket socket) {
                        if (error_code)
                        { // error
                            std::cout << "\n[Server] Connection Error : " << error_code.message();
                        }
                        else
                        {
                            std::cout << "\n[Server] New Connection : " << socket.remote_endpoint();
                            // create connection
                            std::shared_ptr<Connection<T>> connection = std::make_shared<Connection<T>>(Connection<T>::OwnerType::Server, _context, std::move(socket), _messageInQueue);
                            // check whether the client reject the connection
                            if (OnClientConnect(connection))
                            {
                                // hold the connection by storing in the queue
                                _deqConnections.emplace_back(std::move(connection));
                                _deqConnections.back()->ConnectToClient(this, _IDCounter++);
                                std::cout << "\n[Server] " << _deqConnections.back()->GetID() << " Connection Approved";
                            }
                            else
                            {
                                std::cout << "\n[Server]  Connection Denied : " << socket.remote_endpoint();
                            }
                        }

                        WaitForClientConnection();
                    });
            }

            /// @brief send message tp a client
            /// @param client
            /// @param msg
            void MessageClient(std::shared_ptr<Connection<T>> clientConnection, const Message<T> &msg)
            {
                if ((clientConnection && clientConnection->IsConnected()))
                {
                    clientConnection->Send(msg);
                }
                else
                {
                    OnClientDisconnect(clientConnection);
                    clientConnection.reset(); // reset the owner ->nullptr
                    _deqConnections.erase(std::remove(_deqConnections.begin(), _deqConnections.end(), clientConnection), _deqConnections.end());
                }
            }

            void MessageAllClients(const Message<T> &msg, std::shared_ptr<Connection<T>> pIgnoredClient = nullptr)
            {
                bool bInvalidClientExist = false;
                for (std::shared_ptr<Connection<T>> &clientConnection : _deqConnections)
                {
                    if (clientConnection && clientConnection->IsConnected())
                    {
                        if (clientConnection != pIgnoredClient)
                        {
                            clientConnection->Send(msg);
                        }
                    }
                    else // invalid cilent
                    {
                        OnClientDisconnect(clientConnection);
                        clientConnection.reset();
                        bInvalidClientExist = true;
                    }
                }
                if (bInvalidClientExist)
                {
                    _deqConnections.erase(std::remove(_deqConnections.begin(), _deqConnections.end(), nullptr), _deqConnections.end());
                }
            }

            /// @brief
            /// @param maxMessages
            /// @param bWait, if true, loop will continue only if the message comes
            /// @return
            void Update(size_t maxMessages = 1, bool bWait = false)
            {
                // reduce the cpu full load
                if (bWait) _messageInQueue.wait();
                size_t messageCount = 0;
                // check whether have message in messagequeue
                while (messageCount < maxMessages && _messageInQueue.empty() == false)
                {
                    // extract the message from queue
                    auto msg = _messageInQueue.pop_front();

                    // notify message
                    OnMessage(msg.remoteConnection, msg.msg);
                    messageCount++;
                }
            }

            /// @brief call when handshake is complete
            /// @param client
            /// @return
            virtual bool OnCilentValidated(std::shared_ptr<Connection<T>> clientConnection)
            {
                return false;
            }

        protected:
            /// @brief call when a client connection
            /// @param client
            /// @return
            virtual bool OnClientConnect(std::shared_ptr<Connection<T>> clientConnection)
            {
                return false;
            }

            /// @brief call when a client disconnect
            /// @param client
            /// @return
            virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> clientConnection)
            {
                std::cout << "\n[Server] " << clientConnection->GetID() << "Client Disconnected";
            }

            /// @brief called when a message arrives
            /// @param client
            /// @param msg
            virtual void OnMessage(std::shared_ptr<Connection<T>> clientConnection, Message<T> &msg)
            {
            }

        protected:
            Tsqueue<OwnedMessage<T>>                   _messageInQueue;
            asio::io_context                           _context;
            std::thread                                _threadContext;
            std::deque<std::shared_ptr<Connection<T>>> _deqConnections; // contain all valid connection
            asio::ip::tcp::acceptor                    _acceptor;
            uint16_t                                   _portNumber;
            // Client will be identified in the wider system via id
            uint32_t _IDCounter = 1000;
        };
    } // namespace Net
} // namespace Olc