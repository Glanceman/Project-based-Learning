#pragma once

#include "net_message.h"
#include "net_tsqueue.h"
#include "net_common.h"


namespace Olc
{

    namespace Net
    {
        template<typename T>
        class Connection : public std::enable_shard_from_this<Connection<T>>{
        public:
            Connection(){

            }
            virtual ~Connection(){

            }

        public:
            bool ConnectToServer();
            bool Disconnect();
            bool IsConnected() const;
        };
    }
    
}