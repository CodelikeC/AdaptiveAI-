#include <iostream> 
using namespace std; 
#include "core/adaptation.h"
#include "modules/preprocessing/data_cleaner.h"
#include "modules/preprocessing/feature_extraction.h"
#include "modules/communication/rpc_client.h"
#include "utils/logger.h"
#include <map>
#include <string>

int main() {
    using namespace adaptive_ai;

    Logger::info("=== Running Adaptive Core Demo ===");

    // 1. Dữ liệu giả lập
    std::map<std::string, std::string> raw_data = {
        {"user_input", " Hello world! "},
        {"status", "OK"}
    };

    // 2. Làm sạch
    DataCleaner cleaner;
    auto cleaned = cleaner.clean(raw_data);

    // 3. Trích xuất đặc trưng
    FeatureExtractor extractor;
    auto features = extractor.extract_features(cleaned);

    // 4. Gửi lên C2 hoặc hệ thống giám sát
    communication::RpcClient client("127.0.0.1", 9090);
    client.send_request("status");

    // 5. Chạy bộ não thích nghi
    core::Adaptation engine;
    engine.process(features);

    Logger::info("=== Adaptive Core Run Completed ===");
    return 0;
}
