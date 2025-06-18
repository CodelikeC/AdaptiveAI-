#pragma once 
#include <iostream> 
#include <string>
#include <vector> 

#include <algorithm> 

#include "plugin_interface.h"

using namespace std; 

namespace adaptive_ai 
{
    class SensingPlugin : public Plugin
    {
        public: 
        SensingPlugin(); 
        virtual ~SensingPlugin() override;  

        // trien khai cac phuong thuc .. 
        virtual string name () const override;
        virtual void initialize(); 
        virtual void execute() override; 
        virtual void shutdown() override ; 

        private: 
        bool isIntialized; 
        vector<string> sensedData; 

    };
}



