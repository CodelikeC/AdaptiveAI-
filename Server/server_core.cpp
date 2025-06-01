#include "server_core.h"
using namespace std ; 

ServerCore :: ServerCore(const string &nodeId, const string &ip , int port)
: localNode_(nodeId, ip, port), sync_(NodeManager_), p2p_(localNode_){}

ServerCore :: ~ServerCore(){}

void ServerCore :: start()
{
    NodeManager_.addNode(localNode_); 
    sync_.startSync(); 
    cout <<"Server Started on" << localNode_.getIP() << endl; 
}

void ServerCore :: stop()
{
    sync_.stopSync(); 
    cout <<"Server Stopped" << endl; 
}

void ServerCore :: processData(const string &data)
{
    sync_.syncData(localNode_.getID(), data); 
    // co the them logic AI phan tich du lieu .. . 
}