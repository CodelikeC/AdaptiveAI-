#include "../core/sync_manager.h"
using namespace std; 

#include <iostream>
#include <thread> 

#include "../core/sync_manager.cpp"
using namespace adaptive_ai :: net; 

int main ()
{
    SyncManager nodeA, nodeB; 
    nodeA.start("NodeA"); 
    nodeB.start("NodeB");

    nodeB.onSnapshotReceive([](int id, const string &payload){
        cout <<"Callback NodeB - Received snapshot #"
        << id <<"payload:" << payload << endl; 
    }); 

    nodeA.pushSnapshot("NodeB", 42); 
    nodeB.requestSnapshot("NodeA", 99); 

    this_thread :: sleep_for(chrono :: seconds(2)) ; 

    nodeA.stop(); 
    nodeB.stop(); 
    return 0; 
}