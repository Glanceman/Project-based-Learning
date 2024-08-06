#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace Olc
{

    namespace Net
    {
        template<typename T>
        class Client_Interface{
        
        public:
        
            Client_Interface(){

            }
        
            virtual ~Client_Interface(){
                Disconnect();
            }


        public:
            bool Connect(const std::string& hostAddress, const uint16_t portNumber){
                try{


                    asio::ip::tcp::resolver resolver(_asioContext);
                    asio::ip::tcp::resolver::results_type _endpoints =resolver.resolve(hostAddress,std::to_string(portNumber));
                    //create connection
                    _connection = std::make_unique<Connection<T>>(Connection<T>::OwnerType::Client,_asioContext, asio::ip::tcp::socket(_asioContext), _messageQueueIn); // todo
                    _connection->ConnectToServer(_endpoints);

                     _thrContext = std::thread([this]()
                                         { 
                                            /*allow async operation to be able to run without blocking the main thread*/
                                            std::cout << "\nContext Start"; 
                                            _asioContext.run(); 
                                            std::cout << "\nContext finish"; 
                                        }
                                    );

                }catch(std::exception& exception ){
                    std::cout<<"\n Exception "<<exception.what();
                    return false;
                }
                
                return false;
            }

            void Disconnect(){

                if(IsConnected()){
                    _connection->Disconnect();
                }
                
                _asioContext.stop();
                if(_thrContext.joinable()){
                    _thrContext.join();
                }
                _connection.release();
            }

            bool IsConnected(){
                if(_connection){
                    return _connection->IsConnected();
                }
                return false;
            }

            Tsqueue<OwnedMessage<T>>& GetIncomeMessage(){
                return _messageQueueIn;
            }

        protected:
            asio::io_context _asioContext;
            std::thread _thrContext;
            asio::ip::tcp::socket _socket;
            std::unique_ptr<Connection<T>> _connection;

        private:
            //hold all recevied message 
            Tsqueue<OwnedMessage<T>> _messageQueueIn;
        };


    }
}