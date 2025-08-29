#pragma once 
#include <string>
#include <algorithm>

#include <unordered_map>
#include <mutex> 

namespace adaptive_ai
{
    class TrustGuard
    {
        public: 
        TrustGuard(int default_score = 100);
        ~TrustGuard(); 

        void updateTrustScore(const std :: string &module, int delta);
        int getTrustScore(const std :: string &module); 
        bool isTrusted(const std :: string &module);

        private: 
        std :: unordered_map < std::string, int> trustScore_; 
        std :: mutex scoreMutex_;  
        const int TRUST_THRESHOLD_ = 50; 
        int defaultScore_;
    };
}
