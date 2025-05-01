#include <iostream> 
using namespace std ; 
#include <string> 

#include "intrusion_detector.h"
#include <algorithm>


namespace adaptive_ai 
{

IntrusionDetector::IntrusionDetector() 
{
    // Mẫu xâm nhập được huấn luyện thủ công hoặc học qua dữ liệu
    known_patterns = 
    { 
    "suspicious_exec", 
    "unauthorized_access", 
    "port_scan", 
    "malicious_payload" 
    };
}

bool IntrusionDetector::detect_intrusion(const std::map<std::string, std::string>& input_data) 
{
    detected_patterns.clear();
    for (const auto& [key, value] : input_data) 
    {
        if (matches_known_pattern(value)) 
        {
            detected_patterns.push_back(value);
        }
    }
    return !detected_patterns.empty();
}

bool IntrusionDetector::matches_known_pattern(const std::string& data) 
{
    return std::any_of(known_patterns.begin(), known_patterns.end(), 
    [&](const std::string& pattern) 
    {
        return data.find(pattern) 
        != std::string::npos;
    });
}

const std::vector<std::string>& IntrusionDetector
::get_detected_patterns() const 
{
    return detected_patterns;
}

} // namespace adaptive_ai
