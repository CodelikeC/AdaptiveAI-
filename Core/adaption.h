#include <iostream> 
using namespace std;

#include <string> 
#include <algorithm> 

#pragma once

class AdaptiveEngine
{
    public: 
    AdaptiveEngine(); 
    ~AdaptiveEngine(); 

    void initialize() ; 
    void ShutDown(); 
    void adapt(string &enviroment_state);

    private: 
    // Internal state; 
    int adaption_level;
};