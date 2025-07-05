#pragma once 

#include "repc_client.h"
#include <thread>

#include <atomic>
#include <chrono>

namespace communication
{
    class HeartBeatMonitor
    {
        public: 
        HeartBeatMonitor(RpcClient &client); 
        HeartBeatMonitor() = default; 
        ~HeartBeatMonitor(); 
        
        void start(); 
        void stop(); 

        private: 
        void run(); 

        RpcClient &client; 
        atomic<bool>running_; 
        thread monitor_thread; 
    }; 
}