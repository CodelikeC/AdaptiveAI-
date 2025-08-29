#pragma once 

#include <iostream> 
#include <string> 
#include <cmath> 
#include <unordered_map> 

// forward declare lua_State to avoid including Lua Headers in the public header...// 
extern "C"
{
    typedef struct lua_State lua_State;
}

using namespace std; 

class KillSwitchMonitor
{
    public: 
    KillSwitchMonitor(); 
    ~KillSwitchMonitor(); 

    void intialize(); 
    void shutdown(); 

    // theo doi va danh gia logic .. ./ 
    bool monitorLogicIntegrity(const string &logicname, float threatScore); 
    bool evaluateAbortCondition(float threatScore, const string &source ); 

    // Giao tiep voi lua ..///
    bool loadLuaScript(const string &filename); 
    string queryActionPolicy(float score, const string &source); 

    private: 
    lua_State *L = nullptr; 
    const float killThreshold =0.85f; // nguong de kich hoat kill switch..//
    
    void initLua(); 
    void closeLua(); 
}; 


#endif 
