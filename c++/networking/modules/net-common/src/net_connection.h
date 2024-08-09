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
            :_asioContext(asioContext), _socket(std::move(socket)), _messageQueueIn(qIn)
            {
                _ownerType = ownerType;
            }
            virtual ~Connection(){

            }

        public:
            /// @brief client connect to the server
            /// @param endpoints 
            /// @return 
            bool ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints);

            // @brief record the id of the connection to client; in other words: assign id to the client
            /// @param id 
            void ConnectToClient(uint16_t id){
                if(_ownerType==OwnerType::Server){
                    if(_socket.is_open()){
                        _id=id;
                        // wait for the client message 
                        ReadHeader();
                    }
                }
            }

            void Disconnect(){
                if(IsConnected()){
                    asio::post(_asioContext,
                        [this](){
                            _socket.close();
                        }
                    );
                }
            };

            bool IsConnected() const{
                return _socket.is_open();
            };
            bool Send(const Message<T>& msg){
                asio::post(_asioContext,
                    [this,msg](){
                        bool bWritingSocket = _messageQueueOut.empty()?false:true;
                        _messageQueueOut.emplace_back(msg);
                        if(bWritingSocket==false){
                             WriteHeader();//send the packet until queue is empty;
                        }
                    });
                return true;
            };

            // This ID is used system wide - its how clients will understand other clients
			// exist across the whole system.
			uint32_t GetID() const
			{
				return _id;
			}
        private:
            /// @brief Async context ready to read a message header
            void ReadHeader(){
                asio::async_read(_socket,asio::buffer(&_incomingMessageTemp.header, sizeof(Message_header<T>)),
                    [this](std::error_code ec, std::size_t length){
                        if(ec){
                            std::cout<<"\n [ "<<_id<< " ] "<<"Read Header Fail";
                            _socket.close();
                            return;
                        }

                        if(_incomingMessageTemp.header.size>0){
                            _incomingMessageTemp.body.resize(_incomingMessageTemp.header.size);
                            ReadBody();
                        }else{
                            AddToIncomingMessageQueue();
                        }
                    }
                );
            }
            /// @brief Async context ready to read a message body
            void ReadBody(){
                 asio::async_read(_socket,asio::buffer(_incomingMessageTemp.body.data(),_incomingMessageTemp.body.size()),
                    [this](std::error_code ec, std::size_t length){
                        if(ec){
                            std::cout<<"\n [ "<<_id<< " ] "<<"Read Body Fail";
                            _socket.close();
                            return;
                        }
                        AddToIncomingMessageQueue();
                    }
                 );
            }
            /// @brief Async
            void WriteHeader(){
                asio::async_write(_socket,asio::buffer(&_messageQueueOut.front().header,sizeof(Message_header<T>)),
                    [this](std::error_code ec, std::size_t length){
                        if(ec){
                            std::cout<<"\n [ "<<_id<< " ] "<<"Write Header Fail";
                            _socket.close();
                            return;
                        }
                         if(_messageQueueOut.front().body.size()>0){ //check whether the packet has body
                            WriteBody();
                        }else{ // no body
                            _messageQueueOut.pop_front(); //remove;
                            if(_messageQueueOut.empty()==false){
                                WriteHeader();
                            }
                        }
                    }
                );
            }
            /// @brief Async
            void WriteBody(){
                //write the body into the socket which is sending
                  asio::async_write(_socket,asio::buffer(_messageQueueOut.front().body.data(),sizeof(Message_header<T>)),
                    [this](std::error_code ec, std::size_t length){
                        if(ec){
                            std::cout<<"\n [ "<<_id<< " ] "<<"Write body Fail";
                            _socket.close();
                            return;
                        }
                        _messageQueueOut.front();
                        if(_messageQueueOut.empty()==false){// data are writing to send
                                WriteHeader();
                        }
                    }
                );
            }


            void AddToIncomingMessageQueue(){
                switch (_ownerType)
                {
                case OwnerType::Server/* constant-expression */:
                    /* code */
                    {
                        _messageQueueIn.emplace_back({this->shared_from_this()/*remote*/,_incomingMessageTemp/*msg */});
                    }
                    break;
                case OwnerType::Client:
                    {
                        _messageQueueIn.emplace_back({nullptr/*remote*/,_incomingMessageTemp/*msg */});
                    }
                default:
                    break;
                }

                ReadHeader();
            }

        protected:
            //each connection has its own socket
            asio::ip::tcp::socket _socket;
            // shared
            asio::io_context& _asioContext;

            //hold message and sent them out
            Tsqueue<Message<T>> _messageQueueOut;

            Message<T> _incomingMessageTemp;
            Message<T> _outgoingMessageTemp;
            //hold all recevied message 
            Tsqueue<OwnedMessage<T>>& _messageQueueIn;

            OwnerType _ownerType = OwnerType::Server;

            uint32_t _id = 0;
        };
    }
    
}