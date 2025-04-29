#pragma once 
#include <iostream>

#include <string> 
#include <vector> 

using namespace std; 

namespace communication
{
    class RpcClient
    {
        public: 
        RpcClient(const string &server_address, int port); 

        // Gui du lieu va nhan phan hoi tu server .. // 
        string send_request(const string &request); 

        // ping de kiem tra ket noi.. // 
        bool ping_server(); 

        private:
        string server_address; 
        int port_; 

        // Mo phong ket noi , thuc te ban co the mo socket o day ../// 
        bool simulate_connection(); 

        // Mo phong phan hoi tu server.. // 
        string simulate_server_response(const string &request);
    }; // namespace communication .. // 
};
