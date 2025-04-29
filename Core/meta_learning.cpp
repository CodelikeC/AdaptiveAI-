#include "meta_learning.h"
#include "logger.h"
#include <algorithm>
#include <random>
#include <chrono>

namespace adaptive_ai {

MetaLearning::MetaLearning()
{
    learning_rate = 0.01;
    adaptation_speed = 1.0;
}

void MetaLearning::observe_performance(const std::vector<double>& performances)
{
    if (performances.empty()) return;

    double sum = 0.0;
    for (auto p : performances) {
        sum += p;
    }
    double avg = sum / performances.size();
    recent_performance.push_back(avg);

    // Giới hạn bộ nhớ
    if (recent_performance.size() > MAX_HISTORY) {
        recent_performance.erase(recent_performance.begin());
    }
}

void MetaLearning::adjust_learning_strategy()
{
    if (recent_performance.size() < 5) return; // Cần đủ dữ liệu

    double trend = compute_trend();
    
    if (trend > 0.01) {
        learning_rate *= 1.05;
        adaptation_speed *= 1.05;
        Logger::info("Meta-Learning: Improving trend detected. Boosting learning_rate to ", learning_rate);
    } else if (trend < -0.01) {
        learning_rate *= 0.9;
        adaptation_speed *= 0.9;
        Logger::warn("Meta-Learning: Declining trend detected. Reducing learning_rate to ", learning_rate);
    } else {
        Logger::info("Meta-Learning: Stable trend, no adjustment.");
    }

    learning_rate = std::clamp(learning_rate, 0.0001, 0.1);
    adaptation_speed = std::clamp(adaptation_speed, 0.5, 2.0);
}

double MetaLearning::compute_trend() const
{
    if (recent_performance.size() < 2) return 0.0;

    double trend = 0.0;
    for (size_t i = 1; i < recent_performance.size(); ++i) {
        trend += recent_performance[i] - recent_performance[i - 1];
    }
    return trend / (recent_performance.size() - 1);
}

double MetaLearning::get_learning_rate() const
{
    return learning_rate;
}

double MetaLearning::get_adaptation_speed() const
{
    return adaptation_speed;
}

} // namespace adaptive_ai
