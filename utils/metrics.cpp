#include "metrics.h"

namespace adaptive_ai {

void Metrics::start_timer(const std::string& label) {
    start_times[label] = std::chrono::steady_clock::now();
}

void Metrics::stop_timer(const std::string& label) {
    auto now = std::chrono::steady_clock::now();
    auto it = start_times.find(label);
    if (it != start_times.end()) {
        durations[label] = std::chrono::duration<double, std::milli>(now - it->second).count();
    }
}

double Metrics::get_elapsed(const std::string& label) const {
    auto it = durations.find(label);
    return (it != durations.end()) ? it->second : 0.0;
}

} // namespace adaptive_ai
