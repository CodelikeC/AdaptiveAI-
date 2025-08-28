
#ifndef FEATURE_EXTRACTION_H 
#define FEATURE_EXTRACTION_H

#include <string>
#include <vector>
#include <map>

std::vector<double> extract_numeric_features(const std::vector<double>& raw_values);

// Trả về map<string,double> encode key-value pairs thành giá trị [0,1]
std::map<std::string, double> extract_key_value_features(const std::map<std::string, std::string>& raw_data);

// Kết hợp cả numeric + key-value
struct FeatureSet {
    std::vector<double> numeric;
    std::map<std::string, double> key_value;
};

FeatureSet extract_features(const std::vector<double>& numeric_data,
                            const std::map<std::string, std::string>& kv_data);

#endif // FEATURE_EXTRACTION_H

