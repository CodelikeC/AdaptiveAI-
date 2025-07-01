#ifndef threat_analyzer_h 
#define threat_analyzer_h 

#include <iostream> 
#include <string> 
#include <vector>
#include <mutex>
#include <unordered_map>

using namespace std;

enum class ThreatLevel{
    SAFE, 
    LOW, 
    MEDIUM, 
    HIGH, 
    CRITICAL
};

struct ThreatSignal
{
    string source; 
    string description;
    float severityScore; 
    long timestamp; 
}; 

class ThreatAnalyzer
{
    public: 
    ThreatAnalyzer(); 
    ~ThreatAnalyzer();

    void ingestSignal(const ThreatSignal &signal);
    ThreatLevel analyzeThreat(); 
    vector<ThreatSignal> getRecentSignals();
    void clearSignals();
    
    private:
    vector<ThreatSignal>signalBuffer; 
    mutex bufferMutex; 

    float computeAggregateSeverity(); 
    ThreatLevel classifyThreatLevel(float score);
};
#endif 