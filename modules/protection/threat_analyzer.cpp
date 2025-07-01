#include "threat_analyzer.h"
#include <iostream>

#include <chrono>
using namespace std ; 

ThreatAnalyzer :: ThreatAnalyzer(){}

ThreatAnalyzer :: ~ThreatAnalyzer(){}

void ThreatAnalyzer :: ingestSignal(const ThreatSignal &signal)
{
    lock_guard<mutex> lock(bufferMutex); 
    signalBuffer.push_back(signal); 
}

vector<ThreatSignal> ThreatAnalyzer :: getRecentSignals()
{
    lock_guard<mutex> lock(bufferMutex); 
    return signalBuffer;
}

void ThreatAnalyzer :: clearSignals()
{
    lock_guard<mutex> lock(bufferMutex);
    signalBuffer.clear(); 
}

float ThreatAnalyzer :: computeAggregateSeverity()
{
    lock_guard<mutex> lock(bufferMutex); 
    float total = 0.0f;
    for (const auto &sig : signalBuffer){
        total += sig.severityScore;
    }
    return signalBuffer.empty() ?0.0f : total/signalBuffer.size(); 
}

ThreatLevel ThreatAnalyzer::classifyThreatLevel(float score) 
{
    if (score < 0.1f) return ThreatLevel::SAFE;
    if (score < 0.3f) return ThreatLevel::LOW;
    if (score < 0.6f) return ThreatLevel::MEDIUM;
    if (score < 0.85f) return ThreatLevel::HIGH;
    return ThreatLevel::CRITICAL;
}

ThreatLevel ThreatAnalyzer::analyzeThreat() 
{
    float score = computeAggregateSeverity();
    ThreatLevel level = classifyThreatLevel(score);

    string levelStr;
    switch (level) {
        case ThreatLevel::SAFE: levelStr = "SAFE"; break;
        case ThreatLevel::LOW: levelStr = "LOW"; break;
        case ThreatLevel::MEDIUM: levelStr = "MEDIUM"; break;
        case ThreatLevel::HIGH: levelStr = "HIGH"; break;
        case ThreatLevel::CRITICAL: levelStr = "CRITICAL"; break;
    }

    cout << "[ThreatAnalyzer] Aggregate Score: " << score
              << " | Threat Level: " << levelStr << endl;

    return level;
}