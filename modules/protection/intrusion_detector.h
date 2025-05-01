// intrusion_detector.h
#pragma once

#include <string>
#include <vector>
#include <map>

namespace adaptive_ai {

class IntrusionDetector {
public:
    IntrusionDetector();

    // Phân tích và phát hiện hành vi nghi ngờ từ luồng dữ liệu đầu vào
    bool detect_intrusion(const std::map<std::string, std::string>& input_data);

    // Ghi nhận các mẫu xâm nhập đã phát hiện
    const std::vector<std::string>& get_detected_patterns() const;

private:
    std::vector<std::string> known_patterns;
    std::vector<std::string> detected_patterns;

    bool matches_known_pattern(const std::string& data);
};

} // namespace adaptive_ai