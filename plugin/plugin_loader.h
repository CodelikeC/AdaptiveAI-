#pragma once 
#include <iostream>
#include <string> 

#include <vector> 
#include <algorithm> 

using namespace std; 

#include "plugin_interface.h"

namespace adaptive_ai
{
    class PluginManager
    {
        public: 
        void load_plugins(const string *directory); 
        void execute_all();
        void unload_all();
        
        private:
        vector<Plugin*> plugins; 
        vector<void*> handles; 
    }; 
}