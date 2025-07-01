#include "trust_guard.h"

using namespace std; 

#include <string> 
#include <iostream>

TrustGuard :: TrustGuard(){}

TrustGuard :: ~TrustGuard(){}

void TrustGuard :: updateTrustScore(const string &module, int delta)
{
    lock_guard<mutex> lock(scoreMutex); 
    TrustScore[module] +=delta; 

    if (TrustScore[module] <0 ) TrustScore[module] = 0;
    if (TrustScore[module] >100) TrustScore[module] = 100; 
}

int TrustGuard :: getTrustScore(const string &module)
{
    lock_guard<mutex> lock(scoreMutex); 
    if (TrustScore.find(module) == TrustScore.end())  
    return 100; 
    return TrustScore[module]; 
}

bool TrustGuard :: isTrusted(const string &module)
{
    return getTrustScore(module) >= TRUST_THRESHOLD;
}