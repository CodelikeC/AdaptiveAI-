#pragma once

#include <vector>

namespace adaptive_ai {

class MetaLearning
{
public:
    MetaLearning();

    void observe_performance(const std::vector<double>& performances);
    void adjust_learning_strategy();
    
    double get_learning_rate() const;
    double get_adaptation_speed() const;

private:
    double compute_trend() const;

private:
    std::vector<double> recent_performance;
    double learning_rate;
    double adaptation_speed;

    static constexpr size_t MAX_HISTORY = 50;
};

} // namespace adaptive_ai
