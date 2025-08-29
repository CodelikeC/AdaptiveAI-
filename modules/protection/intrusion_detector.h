// intrusion_detector.h
#pragma once
#include "threat_analyzer.h"
#include "self_defense.h"
#include "trust_guard.h"
#include <string>
#include <vector>

namespace adaptive_ai {

struct DetectionResult {
    ThreatLevel level;
    std::vector<std::string> defenseActions;
    int trustScore;
};

class IntrusionDetector {
public:
    IntrusionDetector(ThreatAnalyzer& analyzer,
                      TrustGuard& trustGuard,
                      SelfDefense& selfDefense);

    // Ingest a new signal and process it
    DetectionResult processSignal(const ThreatSignal& signal, const std::string& module);

private:
    ThreatAnalyzer& analyzer_;
    TrustGuard& trustGuard_;
    SelfDefense& selfDefense_;
};

} // namespace adaptive_ai
