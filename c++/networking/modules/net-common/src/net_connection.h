#pragma once

#include "net_message.h"
#include "net_tsqueue.h"
#include "net_common.h"


namespace Olc
{

    namespace Net
    {
        template<typename T>
        class Connection : public std::enable_shared_from_this<Connection<T>> {
        public:
        	// A connection is "owned" by either a server or a client, and its
			// behaviour is slightly different bewteen the two.
			enum class OwnerType
			{
				Server,
				Client
			};
        	// Constructor: Specify Owner, connect to context, transfer the socket
			//Provide reference to incoming message queue
            Connection(OwnerType ownerType,asio::io_context& asioContext, asio::ip::tcp::socket socket, Tsqueue<OwnedMessage<T>>& qIn)
            :_asioContext(asioContext), _socket(socket), _messageQueueIn(qIn)
            {
                _ownerType = ownerType;
            }
            virtual ~Connection(){

            }

        public:
            bool ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);
            bool Disconnect();
            bool IsConnected() const;
            bool Send(const Message<T>& msg){
                return true;
            };

            // This ID is used system wide - its how clients will understand other clients
			// exist across the whole system.
			uint32_t GetID() const
			{
				return id;
			}
        protected:
            //each connection has its own socket
            asio::ip::tcp::socket _socket;
            // shared
            asio::io_context& _asioContext;

            //hold message and sent them out
            Tsqueue<Message<T>> _messageQueueOut;

            //hold all recevied message 
            Tsqueue<OwnedMessage<T>>& _messageQueueIn;

            OwnerType _ownerType = OwnerType::Server;

            uint32_t id = 0;
        };
    }
    
}