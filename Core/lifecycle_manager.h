#pragma once 
#include <iostream>
#include <functional>
#include <vector>

using namespace std; 

namespace adaptive_ai 
{
    namespace core 
    {
        enum class LifeCycleState 
        {
            INIITIALIZING, 
            RUNNING, 
            PAUSED, 
            ROLLING_BACK, 
            SHUTTING_DOWN
        }; 

        class LifeCycleManager
        {
            public: 
            LifeCycleManager(); 

            // Intialize system : validate intergrity , load config...
            void onStageChange(const function<void(LifeCycleState)> &cb);

            // Load / Unload plugin.. // 
            bool loadPlugin(const string &path); 
            bool unloadPlugin(const string &name); 
            bool reloadPlugin(const string &name); 

            // Trigger rollback sequence (delegates to MemoryBridge etc..)
            bool triggerRollback(int snapshotID); 

            LifeCycleState state() const; 

            // Graceful shutdown..//
            void shutdown(); 
        }; 
    } // namespace core 
} // namespace adaptive_ai ///
