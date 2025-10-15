#include <iostream> 
#include <thread> 
#include <chrono>

#include "../modules/communication/communication_manager.h"
#include "../modules/communication/heartbeat_monitor.h"
#include "../modules/communication/channel_factory.h"

#include "../modules/communication/heartbeat_monitor.cpp"
#include "../modules/communication/channel_factory.cpp"
#include "../modules/communication/communication_manager.cpp"

#include "../utils/logger.h"

using namespace std; 
using namespace communication; 

int main ()
{
    string server_address = "127.0.0.1"; 
    int server_port = 8080; 

    // step 1: Tao client RPC va secure channel tu factory..//
    auto rpc_client = ChannelFactory :: create_rpc(server_address, server_port);
    auto secure_channel = ChannelFactory::create_secure(server_address, server_port);

    // step 2: Ping server de kiem tra ket noi ..// 
    cout <<"Pinging server via RpcClient" << endl; 
    if (!rpc_client-> ping_server())
    {
        cerr <<"Error - server is not reachable via RPc" << endl; 
        return 1; 
    }
    
    // step3 : thiet lap kenh bao mat ..// 
    if(!secure_channel -> establish_connection())
    {
        cerr <<"Secure Connection failed" << endl; 
        return 1 ;
    }

    cout <<"Secure channel established" << endl; 

    // step 4: 
    CommunicationManager manager(server_address, server_port); 
    if (!manager.initialize())
    {
        cout <<"Error in CommunicationManager failed to initialize" << endl; 
        return 1;
    }

    // step 5 : Gui du lieu qua manager ..// 
    string test_message = "Adaptive_ai status_request" ;
    cout <<"SEND" << test_message << endl; 
    
    string response = manager.transmit(test_message); 
    cout <<"Recieved:" <<response << endl; 

    // step 6 : 
    HeartBeatMonitor monitor (*rpc_client); 
    monitor.start(); 

    // step 7 : 
    cout <<"INFO - Monitoring connection" << endl; 
    this_thread :: sleep_for(10s); 

    // step 8 :
    monitor.stop();
    manager.shutdown();
    
    cout <<"INFO communication demo finished" << endl; 
    return 0; 
}
