#include <iostream>
using namespace std;

#include <string> 
#include <algorithm>

#include "../modules/communication/repc_client.h"
#include "../modules/communication/secure_channel.h"
#include "../utils/logger.h"

using namespace communication;
using namespace adaptive_ai; 

int main() 
{
    Logger::info("=== TEST COMMUNICATION MODULES ===");

    RpcClient client("127.0.0.1", 8080);
    
    if (client.ping_server()) 
    {
        Logger::info("Ping successful.");
    } 
    else 
    {
        Logger::error("Ping failed.");
    }

    std::string response = client.send_request("get_policy");
    Logger::info("Response from server: ", response);

    SecureChannel secure;
    std::string encrypted = secure.encrypt("hello_adaptive");
    std::string decrypted = secure.decrypt(encrypted);

    Logger::info("Encrypted: ", encrypted);
    Logger::info("Decrypted: ", decrypted);

    if (decrypted == "hello_adaptive") 
    {
        Logger::info("Encryption-Decryption successful.");
    } 
    else 
    {
        Logger::error("Encryption-Decryption failed.");
    }

    Logger::info("=== END TEST ===");
    return 0;
}
