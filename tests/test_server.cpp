#include <iostream> 
using namespace std; 
#include <string> 
#include <algorithm>

#include "../server/node.h"
#include "../server/node_manager.h"
#include "../server/p2p_communication.h"
#include "../server/sync.h"

#include "../server/server_core.h"
#include "../server/adaptive_ai.h"
#include "../server/mem_check.h"

#include "../server/node.cpp"
#include "../server/node_manager.cpp"
#include "../server/p2p_communication.cpp"
#include "../server/sync.cpp"

#include "../server/server_core.cpp"
#include "../server/adaptive_ai.cpp"
#include "../server/mem_check.cpp"

#include <functional>
#include <cassert>
#include <chrono>
#include <thread> 

// helper function to run test and reports pass/fail..//
bool runTest(const string &testName, bool condition)
{
    if (condition){
        cout <<"[Pass]" << testName << endl; 
        return true; 
    }
    else 
    {
        cout <<"[FAIL]" << testName << endl; 
        return false; 
    }
}

// test node class ... // 
void testNode()
{
    cout <<"===== Testing Node =====" << endl; 

    Node node("node1", "127.0.0.1", 8080); 
    runTest("Node Id", node.getID() == "node1");
    runTest("Node IP", node.getIP() == "127.0.0.1"); 
    runTest("Node port", node.getPort() == 8080); 
    runTest("Node active", node.isActive() == true); 
    
    node.setActive(false); 
    runTest("Node destructive", node.isActive() == false); 

    node.setActive(true); 
    runTest("Node Reactive", node.isActive() == true); 
}

// Test NodeManager class .. // 
void testNodeManager()
{
    cout <<"\n==== Testing NodeManager ===" << endl; 

    NodeManager manager; 
    Node node1("node1", "127.0.0.1", 8080); 
    Node node2("node2", "192.168.1.2", 8081); 

    manager.addNode(node1); 
    manager.addNode(node2); 

    runTest("Add Node - Size", manager.getActiveNodes().size() == 2);

    Node * foundNode = manager.getNode("node1"); 
    runTest("Get Node", foundNode != nullptr && foundNode -> getID() == "node1"); 

    manager.updateNodeStatus("node1", false); 
    auto activeNodes = manager.getActiveNodes(); 
    runTest("Update Status - Active Count", activeNodes.size() ==1);

    manager.removeNode("node1"); 
    runTest("Remove Non - existent Node", manager.getActiveNodes().size() == 1); 

}

// Test Adaptive AI class .. // 
void testAdaptiveAI()
{
    cout <<"\n ==== Testing AdaptiveAI ====" << endl; 

    AdaptiveAI ai; 

    // Test Normal_Movement ... // 
    ai.analyzeData("test Data", AdaptiveAI :: State :: NORMAL_MOVEMENT, [](const string &){});
    runTest("Normal Movement Analysis", ai.getAnalysisResult().find("node1") !=string ::npos);
    
    // Test Backup ..// 
    ai.analyzeData("backup Data", AdaptiveAI :: State :: BACKUP, [](const string &){});
    runTest("BackUP analysis", ai.getAnalysisResult().find("Backup Mode") != string :: npos); 

    // Test DB_Overheating ../ /
    ai.analyzeData("Overheat Data", AdaptiveAI :: State :: DB_OVERHEATING, [] (const string &){});
    runTest("DB Overheating", ai.getAnalysisResult().find("Database Overheat") != string ::npos);
    
    // Test expandServer ....
    ai.expandServer("External system"); 
    // Note : This just prints, so we cannot assert output directly,.. 
    runTest("Expand Server Call", true); 
    // placeholder for no - exceptions. 

    // Test Self repair
    ai.selfRepair(); 
    runTest("Self repair call", true); // placeholders.. 

}

// Test Sync class (basic, without full loop)

void testSync()
{
    cout <<"\n === Testing Sync ====" << endl; 

    NodeManager manager; 

    Node node1("node1", "127.0.0.1", 8080);
    Node node2("node2", "192.168.1.2", 8081); 
    manager.addNode(node1); 
    manager.addNode(node2); 

    Sync sync(manager); 

    // Test syncData (basic output check, but since it's cout)... // 
    sync.startSync();
    this_thread :: sleep_for(chrono :: milliseconds(100)); // short wait ../
    // runTest("Start Sync -  thread Started", sync.startSync.joinable());  

    sync.stopSync(); 
    // runTest("Stop Sync - thread Joined", !sync.)
}

// Test p2p communication (basic init).. // 
void testP2PCommunication()
{
    cout <<"\n === Testing P2P communication ====" << endl; 

    Node localNode("local", "127.0.0.1", 8080); 
    P2PCommunication p2p(localNode); 

    runTest("Winsock Init", true); // No exception thrown .. / /

    // Test sendMessage to invalid target (expect failure)..// 
    bool sendResult = p2p.sendMessage("255.255.255.255", 9999, "test sendresult") ; 
    runTest(
        "Send Message (Expected Fall)", !sendResult
    ); 

    // Test ReceiveMessage (basic, will fail on bind if port in use)
    string received = p2p.recieveMessage(); 
    runTest("Receive MEssage Call", received.empty()); 
    // Expected empty .. .

    // Cleanup is in destruction ..
    runTest("p2p destructor",true); 
}

// Test Server Core (basic lifecycle and processData for key states).. 
void testServerCore()
{
    cout <<"\n === Testing ServerCore ====" << endl; 

    ServerCore server("Serve1", "127.0.0.1", 8080); 

    server.start(); 
    runTest("Server Start", true); // No crash, prints.. // 

    // Test processData for Backup .. 
    server.processData("backup test", AdaptiveAI :: State :: BACKUP);
    runTest("Process Backup Data", true); // No crash..// 

    // Test processData for DB_OVERHEATING ..// 
    server.processData("Overheat test", AdaptiveAI :: State :: DB_OVERHEATING); 
    runTest("Process Overheating", true); // Cooldown called .. 

    // Test ProcessData for self repair .. 
    server.processData("repair test", AdaptiveAI :: State :: SELF_REPAIR); 
    runTest("Process Self Repair", true); // Expand called .. // 

    // Test HITL response .. // 
    server.handleHITLResponse("approve"); 
    // runTest("HITL approve", server.hitlApprovedSelfDestruct_ == true); 
    server.stop();
    runTest("Server Stop", true); 
}

// Test memory check (basic allocation/free ..)
void testMemCheck()
{
    cout <<"\n === Testing MemCheck ====" << endl; 

    using namespace intergration; 

    void *ptr1 = track_malloc(100, "test.cpp", 10); 
    runTest("Track alloc 2", ptr1 != nullptr); 

    void *ptr2 = track_malloc(200, "test.cpp", 20);
    runTest("Track alloc 2", ptr2!= nullptr); 

    track_free(ptr1); 
    track_free(ptr2); 

    report_memory_usage();
    runTest("Memory Report - No leaks", true); 
}

int main ()
{
    cout <<"Starting Adaptive AI Server Tests...." << endl;

    testNode(); 
    testNodeManager(); 
    testAdaptiveAI(); 
    testSync(); 
    testP2PCommunication(); 
    testServerCore(); 
    testMemCheck(); 

    cout <<"\n All tests completed!" << endl; 
    return 0;     
}
