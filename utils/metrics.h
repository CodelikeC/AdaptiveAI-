#pragma once

#include <string>
#include <chrono>
#include <unordered_map>

namespace adaptive_ai {

class Metrics {
public:
    void start_timer(const std::string& label);
    void stop_timer(const std::string& label);
    double get_elapsed(const std::string& label) const;

private:
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> start_times;
    std::unordered_map<std::string, double> durations;
};

} // namespace adaptive_ai
