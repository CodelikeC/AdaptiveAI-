#pragma once 

#include <memory>
#include "secure_channel.h"
#include "repc_client.h"
#include <cmath>

namespace communication
{
    class ChannelFactory
    {
        public:
        ChannelFactory(); 
        ~ChannelFactory();
        static unique_ptr<RpcClient> create_rpc(string &address, int port); 
        static unique_ptr<SecureChannel> create_secure(string &address, int port);
    }; 
}