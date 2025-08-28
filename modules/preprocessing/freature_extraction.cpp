/*NEW VERSION*/
#include "feature_extraction.h"
#include <functional>
#include <cmath>
#include <sstream>
#include <limits> 
#include <algorithm> 

std::vector<double> extract_numeric_features(const std::vector<double>& raw_values) {
    if (raw_values.empty()) return {};

    double min_val = *std::min_element(raw_values.begin(), raw_values.end());
    double max_val = *std::max_element(raw_values.begin(), raw_values.end());

    std::vector<double> normalized;
    normalized.reserve(raw_values.size());

    if (std::fabs(max_val - min_val) < std::numeric_limits<double>::epsilon()) {
        // Nếu toàn bộ giá trị giống nhau, cho ra 0.5
        normalized.assign(raw_values.size(), 0.5);
        return normalized;
    }

    for (auto v : raw_values) {
        double norm = (v - min_val) / (max_val - min_val);
        normalized.push_back(norm);
    }
    return normalized;
}

std::map<std::string, double> extract_key_value_features(const std::map<std::string, std::string>& raw_data) {
    std::hash<std::string> hasher;
    std::map<std::string, double> encoded;

    for (const auto& [key, value] : raw_data) {
        auto raw_hash = static_cast<double>(hasher(value) & 0x7FFFFFFF);
        double norm = raw_hash / static_cast<double>(0x7FFFFFFF);
        encoded[key] = norm;
    }
    return encoded;
}

FeatureSet extract_features(const std::vector<double>& numeric_data,
                            const std::map<std::string, std::string>& kv_data) {
    FeatureSet fset;
    fset.numeric = extract_numeric_features(numeric_data);
    fset.key_value = extract_key_value_features(kv_data);
    return fset;
}


