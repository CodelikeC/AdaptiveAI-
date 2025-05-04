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


class ServerCore 
{
private:           

public:
    ServerCore();
    void startServer();
    void processData(const std::vector<string>& data);
    void syncNodes();
};

#endif // SERVER_CORE_H