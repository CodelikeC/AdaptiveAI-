// demo_sensing.h
#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace demo_sensing {

    // Tiền xử lý dữ liệu đầu vào, trả về map đã clean
    std::unordered_map<std::string, std::string> clean_data(
        const std::unordered_map<std::string, std::string>& raw_data);

    // Trích xuất vector đặc trưng từ dữ liệu đã clean
    std::vector<double> extract_features(
        const std::unordered_map<std::string, std::string>& cleaned_data);

} // namespace demo_sensing
