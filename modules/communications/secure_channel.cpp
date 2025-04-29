#include <iostream> 
using namespace std; 

#include "utils/logger.h"
#include "secure_channel.h"

#include <thread> 
#include <chrono>

using namespace adaptive_ai; // de logger hoat dong .. // 

namespace communication
{
    SecureChannel :: SecureChannel(const string &server_address, int port)
    : server_address_(server_address), port_(port), connected_(false)
    {
        Logger :: info("SercureChannel: Created_for" , server_address, ":", port_);

    }

    bool SecureChannel :: establish_connection()
    {
        lock_guard<mutex> lock(connection_mutex_); 

        Logger :: info("Secure Channel : Establishing secure connection... "); 
        this_thread :: sleep_for(chrono :: milliseconds(30)); 

        connected_ = simulate_handshake(); 

        if (connected_)
        {
            Logger :: info("SecureChannel: Secure connection establised", connected_); 

        }
        else 
        {
            Logger :: error("SecureChannel: Failed to established secure connection"); 

        }
        return connected_; 
    }
    bool SecureChannel :: send_secure(const string &data)
    {
        std::lock_guard<std::mutex> lock(connection_mutex_);

    if (!connected_) {
        Logger::warn("SecureChannel: Cannot send, not connected.");
        return false;
    }

    std::string encrypted_data = encrypt(data);

    Logger::debug("SecureChannel: Sending encrypted data: ", encrypted_data);

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Giả lập độ trễ

    return true;
    }

    string SecureChannel :: receive_secure()
    {
        lock_guard<mutex>lock(connection_mutex_); 

        if(!connected_)
        {
            Logger :: warn("SecureChannel: Cannot Receive, not connected"); 
            return "";
        }
            Logger :: debug ("SecureChannel: Recieving encrypted data");

            // Gia lap do tre ..// 
            this_thread :: sleep_for(chrono :: milliseconds(150)); 

            string encrypted_response  = "Encrypted_response_simulation"; 
            string decrypted_response = decrypt(encrypted_response); 

            Logger :: debug("Secure Change : Decrypted response", decrypted_response); 
            return decrypted_response; 

    }

    void SecureChannel :: close()
    {
        lock_guard<mutex> lock(connection_mutex_); 

        if(connected_)
        {
            Logger :: info("SecureChannel: Closing Connection"); 
            connected_= false ;
        }
        else 
        {
            Logger :: warn("SecureChannel: No active connection to server"); 
            return; 
        }
    }

    // ---- Cac ham mo phong chi tiet ---- / / 
    bool SecureChannel :: simulate_handshake()
    {
        // 90 % co hoi handshake thanh cong .. / / 
        return (rand() % 10 < 9); 
    }

    string SecureChannel :: encrypt(const string &plain_text)
    {
        return "Encrypted("+plain_text+")"; 
    }
    
    string SecureChannel :: decrypt(const string &cipher_text)
    {
        if (cipher_text.find("Encrypted" ) == 0)
        {
            return cipher_text.substr(10, cipher_text.length() - 11);
            // bo ma hoa .. 
        }
        return "INVALID_CIPHER"; 
    }
}
