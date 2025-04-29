#include "adaption.h"
using namespace std; 
#include <iostream>

AdaptiveEngine :: AdaptiveEngine() : adaption_level (0) {};
AdaptiveEngine :: ~AdaptiveEngine()
{}

void AdaptiveEngine :: initialize()
{
    cout <<"Adaption Engine initilized :" << endl; 
}; 

void AdaptiveEngine :: adapt(string &enviroment_state)
{
    cout <<"Adaptive Engine : Adapting to:" << endl;
    cout << enviroment_state ; 
    adaption_level++; 
}

void AdaptiveEngine :: ShutDown()
{
    cout <<"[AdaptionEngine] shutting down" << endl; 
}
