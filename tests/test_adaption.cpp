#include <iostream> 
using namespace std; 

#include "../core/adaption.h"
#include "../core/adaption.cpp"

int main ()
{
    cout <<"=============================" << endl;
    cout <<"test adaption for Adaptive AI" << endl;
    cout <<"=============================" << endl;

    // Gọi object cho file adaption .. // 
    AdaptiveEngine engine;
    
    // Mo phong khoi dong he thong .. // 
    engine.initialize();  

    // Mo phong trang thai thay doi .. // 
    string env1 = "High CPU detected"; 
    engine.adapt(env1); 

    string env2 = "Unsual network from port 4444";
    engine.adapt(env2); 
    
    string env3 = "Systems resource levels Back to Normal"; 
    engine.adapt(env3); 

    // tat he thong .. // 
    engine.ShutDown(); 

    cout  <<"Test adaptive completed" << endl; 
    return 0; 
}
