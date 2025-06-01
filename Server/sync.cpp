#include "sync.h"
#include <iostream> 

Sync :: Sync (NodeManager &manager) : 
nodeManager_(manager), running_(false){}

Sync :: ~Sync()
{
    stopSync(); 
}

void Sync :: startSync()
{
    if (!running_)
    {
        running_ = true ; 
        syncThread = thread(&Sync :: syncLoop, this); 
    }
}

void Sync :: stopSync()
{
    if (running_)
    {
        running_ = false;
        if (syncThread.joinable()) syncThread.join();
    }
}

void Sync :: syncData(const string &sourceId, const string &data)
{
    auto activeNodes = nodeManager_.getActiveNodes(); 
    for (const auto &node : activeNodes)
    {
        if (node.getID() != sourceId)
        {
            cout <<"Syncing Data to" << node.getID() << ":" << data << endl; 
        }
    }
} // -> them logic gui du lieu qua p2p 

void Sync :: syncLoop()
{
    while (running_)
    {
        this_thread :: sleep_for(chrono :: seconds(5));
    } // -> Co the them logic phat hien thay doi du lieu .. // 
}