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
            bool initialize(); 
            // Trigger rollback sequence (delegates to MemoryBridge etc..)
            bool triggerRollback(int snapshotID); 

            LifeCycleState state() const; 

            // Graceful shutdown..//
            void shutdown(); 

            private: 
            // Trang thai hien tai cua he thong.. // 
            LifeCycleState state_; 
            // Danh sach plugin da load (key : ten plugin)
            map<string,struct PluginRecord> plugins_; 

            // Danh sach callback duoc dang ky khi thay doi trang thai .. // 
            vector<function<void(LifeCycleState)>> callbacks_; 

            // Mutex bao ve dong bo truy cap state/ plugin..//
            mutable mutex mu_;
        }; 
    } // namespace core 
} // namespace adaptive_ai ///

