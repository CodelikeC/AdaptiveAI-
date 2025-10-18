#include "rollback_evaluator.h"
using namespace std; 

#include <sstream> 
#include <regex> 

#include <algorithm>
#include <cmath>

using namespace std; 
using namespace adaptive_ai :: analysis ; 

namespace {
    struct PolicyWeights{
        double cpuWeight = 0.4; 
        double memWeight = 0.3;
        double errWeight = 0.3; 
    }; 

    PolicyWeights g_policy; 

    // Parse lightweight numeric metric from text or Json-live.. 
    double extractMetric(const string &src, const string &key, double def = 0.0 )
    {
        try {
            regex pattern("\""+ key + R"("\s*:\s*([0-9\.eE\+\-]+))" ); 
            smatch match; 
            if (regex_search(src, match, pattern)){
                return stod(match[1].str()); 
            }
        }catch(...){

        }
        return def; 
    }
    // Normalize positive metric difference (smaller = better)
    double normalizeDiff(double before, double after){
        if (before <=0.0) return 1.0; 
        double ratio = (before - after) / before; 
        ratio = clamp(ratio, -1.0, -1.0); 
        return 0.5 + 0.5 * ratio; // map tp 0-1..// 
    }

    // ========================================= // 
    // Evaluate Rollback Candidate..// 
    // ========================================= // 
}; 
    RollbackScore RollbackEvaluator :: evaluate(int snapshotId, const string &beforeMetrics, const string &afterMetrics){
        RollbackScore result; 
        result.score = 0.0; 
        result.reason = "unknown"; 

        // Extract key metrics .. // 
        double cpuBefore = extractMetric(beforeMetrics, "cpu", 0.0); 
        double cpuAfter  = extractMetric(afterMetrics,  "cpu",  0.0);
        double memBefore = extractMetric(beforeMetrics, "mem", 0.0);

        double memAfter  = extractMetric(afterMetrics,  "mem",  0.0);
        double errBefore = extractMetric(beforeMetrics, "error", 0.0);
        double errAfter  = extractMetric(afterMetrics,  "error", 0.0);

        // Compute normalized improvements..//
        double cpuScore = normalizeDiff(cpuBefore, cpuAfter); 
        double memScore = normalizeDiff(memBefore, memAfter);
        double errScore = normalizeDiff(errBefore, errAfter); 

        // Weight sum...// 
        double total = g_policy.cpuWeight * cpuScore +
                   g_policy.memWeight * memScore +
                   g_policy.errWeight * errScore;

        result.score = clamp(total, 0.0, 1.0); 

        ostringstream oss; 
        oss <<"RollbackEval snapshots" << snapshotId
        << "CPU = " << cpuScore
        <<" MEM = " << memScore 
        <<" ERR = " << errScore
        <<" => Score " << result.score; 
        result.reason  = oss.str(); 

        cout <<"RollbackEvaluator" << result.reason << endl;
        return result; 
    }
    void RollbackEvaluator :: updatePolicy(const string &feedbackError)
    {
        // expect feedback Json like : cpu weight.. 
        double newCpu = extractMetric(feedbackError, "cpuWeight", g_policy.cpuWeight); 
        double newMem = extractMetric(feedbackError, "memWeight", g_policy.memWeight); 
        double newErr = extractMetric(feedbackError, "errWeight", g_policy.errWeight);
        
        // Normalize weights to sum 1 .. /
        double sum = newCpu + newMem + newErr; 

        if (sum <= 0.0) sum = 1.0; 
        g_policy.cpuWeight = newCpu / sum;
    g_policy.memWeight = newMem / sum;
    g_policy.errWeight = newErr / sum;

    cout << "[RollbackEvaluator] Policy updated → "
         << "cpu=" << g_policy.cpuWeight
         << " mem=" << g_policy.memWeight
         << " err=" << g_policy.errWeight << endl;
}