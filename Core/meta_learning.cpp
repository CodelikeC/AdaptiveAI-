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
void MetaLearning :: set_learning_rate(double learning_rate)
{
    this -> learning_rate = learning_rate ; 
}

void MetaLearning :: set_adaption_speed(double adaptation_speed)
{
    this -> adaptation_speed = adaptation_speed;
}
double MetaLearning::get_learning_rate() const
{
    return learning_rate;
}

double MetaLearning::get_adaptation_speed() const
{
    return adaptation_speed;
}

void MetaLearning :: contextual_awareness()
{
    cout <<"Contextual awareness" << endl; 
    cout <<"--------------------" << endl; 

    Logger::info("MetaLearning", "Analyzing contextual relevance of performance data...");

    if (recent_performance.empty()) 
    {
        Logger::warn("MetaLearning", "No performance data available for context analysis.");
        return;
    }

    double last = recent_performance.back();
    if (last > 0.9) 
    {
        Logger::info("MetaLearning", "System is well-aligned with context.");
    } 
    else if (last < 0.5) 
    {
        Logger::warn("MetaLearning", "Performance mismatch with expected context. Adaptation needed.");
    } 
    else 
    {
        Logger::info("MetaLearning", "Moderate contextual alignment.");
    }
}

void MetaLearning :: stability_factor()
{
    cout <<"Stability factor" << endl;
    cout <<"----------------" << endl; 

    Logger::info("MetaLearning", "Evaluating stability of performance...");

    if (recent_performance.size() < 3) 
    {
        Logger::warn("MetaLearning", "Not enough data to evaluate stability.");
        return;
    }

    double mean = 0.0;
    for (auto val : recent_performance) mean += val;
    mean /= recent_performance.size();

    double variance = 0.0;
    for (auto val : recent_performance) 
    {
        variance += (val - mean) * (val - mean);
    }
    variance /= recent_performance.size();

    if (variance < 0.01) 
    {
        Logger::info("MetaLearning", "System is stable.");
    } 
    else 
    {
        Logger::warn("MetaLearning", "High performance variance detected.");
    }
    
}

void MetaLearning :: resource_efficiency()
{
    cout <<"Resource_efficiency" << endl; 
    cout <<"-------------------" << endl; 

    Logger::info("MetaLearning", "Estimating resource efficiency...");

    // Giả định mô phỏng hiệu quả tài nguyên theo learning_rate và adaptation_speed
    double efficiency = (1.0 / learning_rate) * (2.0 - adaptation_speed);

    if (efficiency < 10.0) 
    {
        Logger::info("MetaLearning", "System is highly resource efficient.");
    } 
    else if (efficiency < 50.0) 
    {
        Logger::info("MetaLearning", "System is moderately efficient.");
    } 
    else 
    {
        Logger::warn("MetaLearning", "Resource usage is inefficient. Consider optimization.");
    }
}

string MetaLearning :: feedback_quality()
{
    cout <<"Feedback quality" << endl; 
    cout <<"----------------" << endl;
    
    Logger::info("MetaLearning", "Assessing feedback quality...");

    if (recent_performance.size() < 2) 
    {
        Logger::warn("MetaLearning", "Insufficient data to evaluate feedback quality.");
        return "UNKNOWN";
    }

    double trend = compute_trend();

    if (std::abs(trend) < 0.001) 
    {
        Logger::info("MetaLearning", "Neutral feedback trend.");
        return "NEUTRAL";
    } 
    else if (trend > 0.001) 
    {
        Logger::info("MetaLearning", "Positive feedback detected.");
        return "POSITIVE";
    } 
    else 
    {
        Logger::warn("MetaLearning", "Negative feedback detected.");
        return "NEGATIVE";
    }
}

} // namespace adaptive_ai
