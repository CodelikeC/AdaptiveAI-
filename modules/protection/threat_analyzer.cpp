#include "threat_analyzer.h"

using namespace std; 

namespace adaptive_ai
{
    ThreatAnalyzer :: ThreatAnalyzer() = default; 
    ThreatAnalyzer :: ~ThreatAnalyzer() = default; 

    void ThreatAnalyzer :: ingestSignal(const ThreatSignal &signal)
    {
        lock_guard <mutex> lock(bufferMutex_); 
        signalBuffer_.push_back(signal); 
    } 

    vector <ThreatSignal> ThreatAnalyzer:: getRecentSignal()
    {
        lock_guard<mutex>lock(bufferMutex_); 
        return signalBuffer_; 
    }

    void ThreatAnalyzer :: clearSignals()
    {
        lock_guard<mutex> lock(bufferMutex_);
        signalBuffer_.clear(); 
    }

    float ThreatAnalyzer :: computeAggregateServerity()
    {
        lock_guard<mutex> lock(bufferMutex_); 
        float total = 0.0f; 
        for (const auto & s : signalBuffer_)
        total += s.serverityScore;
        return signalBuffer_.empty() ? 0.0f : total/signalBuffer_.size();
    }

    ThreatLevel ThreatAnalyzer::classifyThreatLevel(float score)
    {
        if (score < 0.1f) 
        return ThreatLevel :: SAFE; 
        if (score < 0.2f) 
        return ThreatLevel :: LOW;
        if (score < 0.3f) 
        return ThreatLevel :: MEDIUM;
        if (score < 0.4f) 
        return ThreatLevel :: HIGH;
        
        if (score < 0.5f) 
        
        return ThreatLevel :: CRITICAL;
    }

    ThreatLevel ThreatAnalyzer  :: analyzeThreat()
    {
        float score = computeAggregateServerity(); 
        return classifyThreatLevel(score); 
    }
}
