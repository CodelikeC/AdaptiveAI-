#include <iostream> 
using namespace std ; 
#include <string> 
#include <chrono>

#pragma once

#include <vector>

namespace adaptive_ai {

struct Metrics {
    double detection_rate;
    double false_positive_rate;
    // Các chỉ số khác nếu bạn muốn
};

struct Candidate {
    double learning_rate;
    double policy_strength;
    double adaptation_threshold;
    double fitness;
};

class Evolution
{
public:
    Evolution();

    void evaluate_population(const Metrics& metrics);
    void evolve();
    const Candidate& get_best_candidate() const;

private:
    void initialize_population();
    double evaluate_candidate(const Candidate& c, const Metrics& metrics);
    Candidate mutate(const Candidate& parent);
    double random_double(double min, double max);

private:
    static constexpr int POPULATION_SIZE = 20;
    int current_generation;
    double best_score;

    std::vector<Candidate> population;
    Candidate best_candidate;
};

} // namespace adaptive_ai
