#include "trust_guard.h"
#include <string> 

using namespace std; 

namespace adaptive_ai
{
    TrustGuard :: TrustGuard(int default_score) : defaultScore_(default_score){}
    TrustGuard :: ~TrustGuard() = default; 

    void TrustGuard :: updateTrustScore(const string &module, int delta)
    {
    std::lock_guard<std::mutex> lock(scoreMutex_);
    int& s = trustScore_[module]; // creates if missing, default-initialized 0
    if (s == 0) s = defaultScore_; // ensure new module starts at default
    s += delta;
    if (s < 0) s = 0;
    if (s > 100) s = 100;
    }

    int TrustGuard::getTrustScore(const std::string& module) {
    std::lock_guard<std::mutex> lock(scoreMutex_);
    auto it = trustScore_.find(module);
    if (it == trustScore_.end()) return defaultScore_;
    return it->second;
    }

bool TrustGuard::isTrusted(const std::string& module) {
    return getTrustScore(module) >= TRUST_THRESHOLD_;
}

}

