#pragma once 
#include <string>
#include <algorithm>
#include <vector> 
#include <ctime> 
#include <chrono>
#include <iostream> 
#include <mutex> 

namespace adaptive_ai
{
    enum class ThreatLevel 
    {
        SAFE, 
        LOW,
        MEDIUM, 
        HIGH, 
        CRITICAL 
    };

    struct ThreatSignal
    {
        std :: string response; 
        std :: string description; 
        float serverityScore; 

        std ::chrono :: system_clock :: time_point timestamp; 
    }; 

    class ThreatAnalyzer
    {
        public: 
        ThreatAnalyzer(); 
        ~ThreatAnalyzer(); 

        void ingestSignal(const ThreatSignal &signal);
        // purely computes current threat level (deterministic)...// 
        ThreatLevel analyzeThreat();  

        std :: vector<ThreatSignal>getRecentSignal(); 
        void clearSignals(); 

        private: 
        std::vector<ThreatSignal> signalBuffer_; 
        std :: mutex bufferMutex_; 

        float computeAggregateServerity(); 
        ThreatLevel classifyThreatLevel(float score);
    }; 
} // namespace adaptive_ai ...// 
