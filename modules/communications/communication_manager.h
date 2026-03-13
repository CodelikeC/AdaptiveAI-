#pragma once 
#include "repc_client.h"
#include "secure_channel.h"

namespace communication
{
    class CommunicationManager
    {
        public: 
        CommunicationManager(const string &address, int port);
        CommunicationManager(); 
        ~CommunicationManager(); 
        
        bool initialize(); // Thiet lap rpc va secure channel
        string transmit(const string &request); 
        void shutdown(); // dong toan bo kenh .. // 

        private: 
        RpcClient rpc_client; 
        SecureChannel secure_channel; 
    }; 
}