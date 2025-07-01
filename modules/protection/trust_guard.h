#ifndef TRUST_GUARD_H 
#define TRUST_GUARD_H 

#include <string> 
#include <unordered_map>
#include <mutex>

using namespace std ; 

class TrustGuard
{
    public: 
    TrustGuard(); 
    ~TrustGuard(); 

    void updateTrustScore(const string &module, int delta); 
    int getTrustScore(const string &module) ; 
    bool isTrusted(const string &module); 

    private: 
    unordered_map<string, int> TrustScore; 
    mutex scoreMutex; 
    const int TRUST_THRESHOLD = 50; 
}; 

#endif // Trust_GUARD_H 