#include "feature_extraction.h"
#include <functional>
#include <cmath>
#include <sstream>

namespace feature {

std::vector<double> FeatureExtractor::extract_numeric_features(const std::vector<double>& data) {
    std::vector<double> result;
    for (double val : data) {
        result.push_back(val);  // Tùy logic: có thể thêm z-score, log(val), val^2, v.v.
    }
    return result;
}

std::vector<double> FeatureExtractor::extract_key_value_features(const std::unordered_map<std::string, std::string>& data) {
    std::vector<double> features;

    for (const auto& [key, value] : data) {
        double encoded = encode_string_to_number(value);
        features.push_back(encoded);
    }

    return features;
}

double FeatureExtractor::encode_string_to_number(const std::string& value) {
    // Tạm thời: hash string (thay bằng encode one-hot, binary embedding hoặc BERT sau)
    // return static_cast<double>(hash_string(value) % 10000) / 10000.0;
    std::hash<std::string> hasher;
    auto raw_hash = static_cast<double>(hasher(value) & 0x7FFFFFFF); // Giữ dương
    return raw_hash / static_cast<double>(0x7FFFFFFF); // Normalize về [0,1]
}

double FeatureExtractor::hash_string(const std::string& input) {
    std::hash<std::string> hasher;
    return static_cast<double>(hasher(input));
}

} // namespace feature
