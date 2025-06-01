#ifndef ADAPTIVE_AI_H 
#define ADAPTIVE_AI_H 

#include <string> 
#include <vector> 
#include <functional>

using namespace std; 

class AdaptiveAI 
{
    public: 
    enum class State
    {
        NORMAL_MOVEMENT, 
        INACTIVE, 
        CRASH, 
        UNSTABLE_MOVEMENT, 
        BACKUP, 
        TOTAL_FAILURE, 
        CORRUPTED_DATA, 
        SERVER_LOST_CONTROL, 
        DB_OVERHEATING, 
        SELF_REPAIR
    };

    AdaptiveAI(); 
    ~AdaptiveAI();  

    void analyzeData(const string &rawData, State state, function <void(const string &)> hitlCallback);
    string getAnalysisResult() const;
    void requestHITL(const string &issue);
    void selfRepair(); 
    void expandServer(const string &externalSystem); 

    private: 
    string analysisResult_ ; 
    bool isOverheating_; 
    bool isRepairing_; 
    function<void(const string &)> hitlCallback_;
};

#endif 