#include "heartbeat_monitor.h"
#include "repc_client.h"
#include <iostream>

namespace communication
{
    HeartBeatMonitor :: HeartBeatMonitor(RpcClient &client)
    : client(client), running_(false){}

    HeartBeatMonitor :: ~HeartBeatMonitor()
    {
        cout <<"End" << endl; 
        stop(); 
    }

    void HeartBeatMonitor :: start()
    {
        running_ = true; 
        if (monitor_thread.joinable())
        {
            monitor_thread.join(); 
        }
    }

    void HeartBeatMonitor :: run()
    {
        using namespace chrono_literals;
        while (running_)
        {
            if (!client.ping_server())
            {
                cerr <<"[HEARTBEAT] Server unreachable!" << endl ; 
            }
            this_thread :: sleep_for(3s); 
        }
    }

    void HeartBeatMonitor :: stop()
    {
        if (running_ == false)
        {
            return;  
        }
    }
}