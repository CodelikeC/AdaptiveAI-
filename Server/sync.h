#ifndef SYNC_H
#define SYNC_H 

#include <iostream> 
using namespace std; 
#include <string> 

#include <thread> 
#include <atomic>

#include "node_manager.h"

class Sync
{
    public: 
    Sync(NodeManager &manager); 
    ~Sync(); 

    void startSync(); 
    void stopSync(); 
    void syncData(const string &sourceId, const string &data );

    private:
    NodeManager &nodeManager_; 
    atomic<bool> running_; 
    thread syncThread; 

    void syncLoop(); 
}; 


#endif 
