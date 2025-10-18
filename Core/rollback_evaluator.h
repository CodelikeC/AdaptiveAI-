#pragma once 
#include <string> 
using namespace std; 

#include <iostream>
#include <vector>
#include <functional>

namespace adaptive_ai 
{
    namespace analysis
    {
        struct RollbackScore
        {
            double score; // normalized 0.1 
            string reason; 
        }; 

        class RollbackEvaluator
        {
    public: 
    // Evaluate a candidate rollback using pre/post metrics...// 
    RollbackScore evaluate(int snapshotId, const string &beforeMetrics, const string &afterMetrics);

    // Update scoring policy based on historic results (learning).. // 
    void updatePolicy(const string &feedbackJson); 

        }; 
    } // namespace analysis.. 
} // namespace adaptive_ai ...