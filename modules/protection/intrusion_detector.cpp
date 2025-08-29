#include "intrusion_detector.h"

namespace adaptive_ai {

IntrusionDetector::IntrusionDetector(ThreatAnalyzer& analyzer,
                                     TrustGuard& trustGuard,
                                     SelfDefense& selfDefense)
    : analyzer_(analyzer), trustGuard_(trustGuard), selfDefense_(selfDefense) {}

DetectionResult IntrusionDetector::processSignal(const ThreatSignal& signal,
                                                 const std::string& module) {
    analyzer_.ingestSignal(signal);

    ThreatLevel level = analyzer_.analyzeThreat();

    // Update trust score: lower trust if signal is severe
    int delta = (signal.serverityScore > 0.7f) ? -10 : +1;
    trustGuard_.updateTrustScore(module, delta);

    std::vector<std::string> defenseActions;
    if (level == ThreatLevel::HIGH || level == ThreatLevel::CRITICAL) {
        defenseActions = selfDefense_.conunter_instrusion({module});
    }

    int trust = trustGuard_.getTrustScore(module);
    return {level, defenseActions, trust};
}

} // namespace adaptive_ai
