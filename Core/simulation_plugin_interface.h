#pragma once 
#include <string> 
#include <functional>
#include <variant> 
#include <vector> 
#include <iostream>

using namespace std; 

namespace adaptive_ai 
{
    namespace simulation
    {
        using SimPayLoad = variant<string, vector<uint8_t>>;

        class SimulationPluginInterface
        {
            public: 
            virtual ~SimulationPluginInterface() = default; 

            // Called when sim supplies a step input or event .. 
            virtual void onSimstep(const SimPayLoad &payload) = 0 ; 

            // Called when plugin is loaded .. 
            virtual bool onLoad(const string &configjson) = 0; 

            // Called when plugin is unloaded.. 
            virtual void onUnload() = 0; 

            // Send control back to simulator (optional).. 
            virtual void sendControl(const string &controlJson) = 0; 
        }; 
    }
}