#include <iostream> 
using namespace std; 
#include "core/adaptation.h"
#include "modules/preprocessing/data_cleaner.h"
#include "modules/preprocessing/feature_extraction.h"
#include "modules/communication/rpc_client.h"
#include "utils/logger.h"

#include <cctype> 
#include <map>
#include <string>

#include <thread> 
#include <chrono>

using namespace preprocessing;
using namespace feature; 

using namespace communication;

int main() {
    using namespace adaptive_ai;

    Logger::info("=== Running Adaptive Core Demo ===");

    // 1. Dữ liệu giả lập
    std::map<std::string, std::string> raw_data = {
        {"user_input", " Hello world! "},
        {"status", "OK"}, 
        {"user_id", "user123"}, 
        {"command", "get_policy"},  
        {"misc", ""}
    
    };

    Logger :: debug("raw data received:");
    for (const auto &[k, v] : raw_data)
    {
        Logger :: debug("key :", k, ", value", v); 
    }

    // 2. Làm sạch
    DataCleaner cleaner;
    auto cleaned = cleaner.clean(raw_data);

    Logger :: info("cleaned Data:");
    for (const auto &[k,v] : raw_data)
    {
        Logger :: info("Key:", k, ", vlaue", v); 
    }

    // 3. Trích xuất đặc trưng
    FeatureExtractor extractor;
    auto features = extractor.extract_features(cleaned_data);

    Logger :: info("Extracted Features:"); 
    for (const auto &[k, v]: features)
    {
        Logger :: info("Feature:", k, ",value", v); 
    }

    // 4. Gửi lên C2 hoặc hệ thống giám sát
    communication::RpcClient client("127.0.0.1", 9090);
    client.send_request("status");

    if (client.ping_server())
    {
        string policy = client.send_request("Get policy"); 
        Logger :: info("Policy received", policy); 

    } 
    else{
        Logger :: warn(
            "Sever not reachable, skipping policy sync"
        ); 
    }

    // 5. Chạy bộ não thích nghi
    core::Adaptation engine;
    engine.process(features);

    Logger::info("=== Adaptive Core Run Completed ===");
    return 0;
}
