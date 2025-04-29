#include "repc_client.h"
#include "utils/logger.h"

#include <iostream> 
#include <thread>
#include <chrono> 

using namespace std;

namespace communication 
{
    RpcClient::RpcClient(const std::string& server_address, int port)
        : server_address(server_address), port_(port)
    {
        adaptive_ai:: Logger::info("RpcClient: Initialized with " + server_address + ":" + std::to_string(port));
    }
    
    bool RpcClient::ping_server()
    {
        adaptive_ai::Logger::debug("RpcClient: Pinging server...");
        return simulate_connection();
    }
    
    std::string RpcClient::send_request(const std::string& request)
    {
        if (!simulate_connection()) {
            adaptive_ai::Logger::error("RpcClient: Failed to connect to server.");
            return "ConnectionError";
        }
    
        adaptive_ai::Logger::info("RpcClient: Sending request - " + request);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Giả lập độ trễ
    
        std::string response = simulate_server_response(request);
        adaptive_ai::Logger::debug("RpcClient: Received response - " + response);
    
        return response;
    }
    
    bool RpcClient::simulate_connection()
    {
        // Mô phỏng 90% thành công kết nối
        return (rand() % 10 < 9);
    }
    
    std::string RpcClient::simulate_server_response(const std::string& request)
    {
        // Mô phỏng phản hồi đơn giản
        if (request == "status") return "OK";
        if (request == "get_policy") return "policy_v1.2";
        return "unknown_command";
    }
    
} // namespace communication...// 