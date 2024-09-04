#pragma once

#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace Olc
{
    namespace Net
    {
        template <typename T>
        class Client_Interface
        {
        public:
            Client_Interface() {}

            virtual ~Client_Interface()
            {
                Disconnect();
            }

        public:
            bool Connect(const std::string &hostAddress, const uint16_t portNumber)
            {
                try
                {
                    // Turn domain name to IP address
                    asio::ip::tcp::resolver resolver(_asioContext);
                    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(hostAddress, std::to_string(portNumber));

                    // Create connection
                    _connection = std::make_unique<Connection<T>>(Connection<T>::OwnerType::Client, _asioContext, asio::ip::tcp::socket(_asioContext), _messageQueueIn);

                    _connection->ConnectToServer(endpoints);

                    _thrContext = std::thread([this]()
                                              { 
                        // Allow async operation to be able to run without blocking the main thread
                        std::cout << "\nContext Start"; 
                        _asioContext.run(); 
                        std::cout << "\nContext finish"; });

                    return true; // Return true if connection is successful
                }
                catch (std::exception &exception)
                {
                    std::cerr << "\nException: " << exception.what() << std::endl;
                    return false;
                }
            }

            void Disconnect()
            {
                if (IsConnected())
                {
                    _connection->Disconnect();
                }

                _asioContext.stop();
                if (_thrContext.joinable())
                {
                    _thrContext.join();
                }
                _connection.reset(); // Use reset() instead of release()
            }

            bool IsConnected()
            {
                return _connection && _connection->IsConnected();
            }

            Tsqueue<OwnedMessage<T>> &GetIncomingMessages() // Renamed for clarity
            {
                return _messageQueueIn;
            }

            // Send message to server
            void Send(const Message<T> &msg)
            {
                if (IsConnected())
                    _connection->Send(msg);
            }

            // Retrieve queue of messages from server
            Tsqueue<OwnedMessage<T>> &Incoming()
            {
                return _messageQueueIn;
            }

        protected:
            asio::io_context _asioContext;
            std::thread _thrContext;
            // Removed _socket as it's not used
            std::unique_ptr<Connection<T>> _connection;

        private:
            // Hold all received messages
            Tsqueue<OwnedMessage<T>> _messageQueueIn;
        };
    }
}