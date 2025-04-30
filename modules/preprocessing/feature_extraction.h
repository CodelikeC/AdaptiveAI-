#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace feature {

class FeatureExtractor {
public:
    // Dành cho dữ liệu số
    std::vector<double> extract_numeric_features(const std::vector<double>& data);

    // Dành cho dữ liệu dạng key-value
    std::vector<double> extract_key_value_features(const std::unordered_map<std::string, std::string>& data);

private:
    double encode_string_to_number(const std::string& value);
    double hash_string(const std::string& input);
};

} // namespace feature
