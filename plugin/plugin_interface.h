#pragma once 
#include <iostream> 
using namespace std;
#include <string> 

#include <algorithm>
#include <vector> 

namespace adaptive_ai
{
    class Plugin
    {
        public: 
        virtual ~Plugin() {};
        virtual string name() const =  0; 
        virtual void intialize() = 0; 
        virtual void execute() = 0; 
        virtual void shutdown() = 0;  
    };
}