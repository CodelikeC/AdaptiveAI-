#pragma once 

#include <iostream> 
using namespace std; 

#include <string> 
#include <mutex> 

namespace communication
{
    class SecureChannel
    {
        public: 
        SecureChannel(const string &server_address, int port); 

        bool establish_connection();  // thiet lap ket noi an toan .. // 

        string receive_secure(); // nhan du lieu an toan . . 

        void close(); // dong ket noi .. // 

        bool send_secure(const string &data);

        private: 
        string server_address_; 
        int port_; 
        bool connected_;
        mutex connection_mutex_; 

        // Cac ham mo phong chi tiet ben trong ..//
        bool simulate_handshake(); 
        string encrypt(const string &plain_text); 
        string decrypt(const string &cipher_text); 
    };
}
