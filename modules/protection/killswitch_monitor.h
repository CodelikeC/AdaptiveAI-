#ifndef killswitch_monitor_h 
#define killswitch_monitor_h 

#include <iostream>
#include <string> 
#include <cmath>
#include <lua.h>

#include <unordered_map>

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
    lua_State *L; 
    const float killThreshold =0.85f; // nguong de kich hoat kill switch..//

    void initLua(); 
    void closeLua(); 
}; 

#endif 