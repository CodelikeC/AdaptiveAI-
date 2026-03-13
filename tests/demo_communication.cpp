#include <iostream> 
#include <thread> 
#include <chrono>

#include "../modules/communications/communication_manager.h"
#include "../modules/communications/heartbeat_monitor.h"
#include "../modules/communications/channel_factory.h"

#include "../modules/communications/heartbeat_monitor.cpp"
#include "../modules/communications/channel_factory.cpp"
#include "../modules/communications/communication_manager.cpp"

#include "../utils/logger.h"

using namespace std; 
using namespace adaptive_ai;
using namespace communication; 

    int main()
    {
        cout << "========================" << endl; 
        cout << "Test Communication Module - AIC\n"; 
        cout << "=========================" << endl; 

        // ============== 1. Test: Channel Factory ============== /// 
        cout <<"[Test 1] -- Channel Factory Creations" << endl; 
        ChannelFactory factory; 

        string server_address =  "127.0.0.1"; 
        int server_port = 8080; 

        auto rpc = factory.create_rpc(server_address, server_port);
        auto secure = factory.create_secure(server_address, server_port); 

        cout <<" RPC client and Secure Channel created successfully" << endl; 

        // ================ 2. Tests 2: Communication Manager =================== // 
        cout <<"[Test 2] - Communication Manager Initialization" << endl; 
        CommunicationManager manager(server_address, server_port); 

        bool init_success = manager.initialize(); 
        if (init_success){
            cout << "Communication Manager initialized successfully" << endl; 
        }
        else  
        {
            cout <<"X Initialization failed (simulated) " << endl; 
        }

        // Test transmit.. // 
        string request = "Hello from AdaptiveAI - Test message"; 
        string response = manager.transmit(request);

        cout << "Request :" << request << endl; 
        cout <<"Response:" << response << endl << endl; 

        // ================ Test 3 : Heartbeat Monitor =================== // 
        cout <<"Test 3 - Heartbeat Monitor" << endl; 
        HeartBeatMonitor monitor(*rpc); 

        cout <<"Starting hearbeat monitor (will run for 6 seconds)... \n";
        monitor.start(); 

        // simulate running for a short time.. 
        this_thread :: sleep_for (chrono :: seconds(6));

        cout << "Stopping hearbeat monitor" << endl; 
        monitor.stop(); 

        cout << "Heart beat Monitor test completed" << endl; 

        // =========== Shutdown =================// 
        cout <<"Final shutting down Communication Manager" << endl; 
        manager.shutdown(); 

        cout << "\n===============================\n" ; 
        cout << "ALL COMMUNICATION TEST COMPLETED" << endl ; 
        cout << "==================================\n"; 

        return 0; 
    }


// int main ()
// {
//     // string server_address = "127.0.0.1"; 
//     // int server_port = 8080; 

//     // // step 1: Tao client RPC va secure channel tu factory..//
//     // auto rpc_client = ChannelFactory :: create_rpc(server_address, server_port);
//     // auto secure_channel = ChannelFactory::create_secure(server_address, server_port);

//     // // step 2: Ping server de kiem tra ket noi ..// 
//     // cout <<"Pinging server via RpcClient" << endl; 
//     // if (!rpc_client-> ping_server())
//     // {
//     //     cerr <<"Error - server is not reachable via RPc" << endl; 
//     //     return 1; 
//     // }
    
//     // // step3 : thiet lap kenh bao mat ..// 
//     // if(!secure_channel -> establish_connection())
//     // {
//     //     cerr <<"Secure Connection failed" << endl; 
//     //     return 1 ;
//     // }

//     // cout <<"Secure channel established" << endl; 

//     // // step 4: 
//     // CommunicationManager manager(server_address, server_port); 
//     // if (!manager.initialize())
//     // {
//     //     cout <<"Error in CommunicationManager failed to initialize" << endl; 
//     //     return 1;
//     // }

//     // // step 5 : Gui du lieu qua manager ..// 
//     // string test_message = "Adaptive_ai status_request" ;
//     // cout <<"SEND" << test_message << endl; 
    
//     // string response = manager.transmit(test_message); 
//     // cout <<"Recieved:" <<response << endl; 

//     // // step 6 : 
//     // HeartBeatMonitor monitor (*rpc_client); 
//     // monitor.start(); 

//     // // step 7 : 
//     // cout <<"INFO - Monitoring connection" << endl; 
//     // this_thread :: sleep_for(10s); 

//     // // step 8 :
//     // monitor.stop();
//     // manager.shutdown();
    
//     // cout <<"INFO communication demo finished" << endl; 
//     // return 0; 
// }
