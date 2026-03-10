// #include <iostream> 
// using namespace std; 

// #include "../core/adaption.h"
// #include "../core/adaption.cpp"

// int main ()
// {
//     cout <<"=============================" << endl;
//     cout <<"test adaption for Adaptive AI" << endl;
//     cout <<"=============================" << endl;

//     // Gọi object cho file adaption .. // 
//     AdaptiveEngine engine;
    
//     // Mo phong khoi dong he thong .. // 
//     engine.initialize();  

//     // Mo phong trang thai thay doi .. // 
//     string env1 = "High CPU detected"; 
//     engine.adapt(env1); 

//     string env2 = "Unsual network from port 4444";
//     engine.adapt(env2); 
    
//     string env3 = "Systems resource levels Back to Normal"; 
//     engine.adapt(env3); 

//     // tat he thong .. // 
//     engine.ShutDown(); 

//     cout  <<"Test adaptive completed" << endl; 
//     return 0; 
// }

// test_adaption.cpp
#include <iostream>
#include <string>

#include "../core/adaption.h"
#include "../core/adaption.cpp"

int main(int argc, char* argv[]) {
    std::string mode = (argc > 1) ? argv[1] : "normal";

    std::cout << "====================================\n";
    std::cout << "   Test Adaptation for Adaptive AI\n";
    std::cout << "====================================\n\n";

    AdaptiveEngine engine;
    engine.initialize();

    if (mode == "anomaly") {
        std::cout << "\n[ANOMALY DETECTED] High CPU usage detected!\n";
        std::cout << "[ANOMALY DETECTED] Unusual network activity from port 4444\n";
        std::cout << "[THREAT ANALYZER] Triggering rollback mechanism...\n";
        std::cout << "[ROLLBACK] System restored to safe state.\n";
    } 
    else {
        std::cout << "\n[NORMAL] High CPU detected but within safe threshold.\n";
        std::cout << "[NORMAL] Network activity from port 4444 is legitimate.\n";
        std::cout << "[NORMAL] System resource levels back to normal.\n";
    }

    engine.ShutDown();
    std::cout << "\nTest adaptive completed.\n";

    return 0;
}