#include <iostream>
using namespace std; 
#include <string> 

// server/server_core.h
#ifndef SERVER_CORE_H
#define SERVER_CORE_H

#include <string>
#include <vector>
#include "../core/adaption.h"
#include "../modules/sensor/network_sensor.h"
#include "../modules/communication/repc_client.h"

#include "node_manager.h"
#include "node.h"
#include "sync.h"
#include "p2p_communication.h"
#include "adaptive_ai.h"

class ServerCore 
{
private:           
Node localNode_; 
NodeManager NodeManager_;
Sync sync_; 
P2PCommunication p2p_; 
AdaptiveAI adaptiveAi_; 
bool hitlApprovedSelfDestruct_;

public:
    ServerCore(const string &nodeId, const string &ip, int port);
    ~ServerCore() ; 
    void start();
    void processData(const string& data, AdaptiveAI :: State state);
    void stop();
    void handleHITLResponse(const string &response); 
    void cooldownDatabase(); 
    void expandServer(const string &externalSystem);
};

#endif // SERVER_CORE_H
