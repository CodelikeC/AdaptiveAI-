#include <iostream> 
using namespace std; 

#include <string> 
#include "policy.h"
#include "utils/logger.h"

#include <numeric>
#include <cmath>
namespace adaptive_ai
{
    Policy :: Policy()
    {
        // Khoi tao ngau nhien mot trong so .. // 
        weights = {0.5, 0.5, 0.5}; 
        // Tam mac dinh 3 trong so = nhau 
    }

    string Policy :: decide_action(const vector<double> &state)
    {
        if (state.empty() or state.empty() != weights.size())
        {
            Logger :: error("Policy : Invalid state size/"); 
            return "invalid";
        }

        double score = compute_score (state); 

        if (score > 0.7)
        {
            Logger :: info("Policy : Decided to Perform action. Score:", score);
            return "perform_action"; 
        }
        else if (score > 0.4)
        {
            Logger :: warn("Policy : Decided to Monitor more. Score:", score) ; 
            return "monitor"; 
        }
        else 
        {
            Logger :: warn("Policy : Decided to wait", score); 
            return "wait"; 
        }
    }

    void Policy :: update_policy(const vector<double> &new_weights)
    {
        if (new_weights.empty() or new_weights.size() != weights.size())
        {
            Logger :: error("Policy : Invalid new weights size."); 
            return ; 
        }
    weights = new_weights;
    Logger::info("Policy: Updated policy weights.");
    }

    double Policy::compute_score(const std::vector<double>& state) const
    {
    double dot_product = std::inner_product(state.begin(), state.end(), weights.begin(), 0.0);
    double norm_factor = std::sqrt(std::inner_product(weights.begin(), weights.end(), weights.begin(), 0.0));
    return dot_product / (norm_factor + 1e-8); // Thêm epsilon để tránh chia cho 0
    }
}
