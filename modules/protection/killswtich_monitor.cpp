#include "killswitch_monitor.h"
#include <iostream>
#include <lauxlib.h>
#include <lualib.h>
#include <string> 
using namespace std; 

KillSwitchMonitor :: KillSwitchMonitor()
{
    initLua(); 
}

KillSwitchMonitor :: ~KillSwitchMonitor()
{
    shutdown(); 
}

void KillSwitchMonitor :: initLua()
{
    L = luaL_newstate(); 
    luaL_openlibs(L); 
    loadLuaScript("Plugin/lua/killswitchNewLogic.lua"); 
}

void KillSwitchMonitor :: shutdown()
{
    closeLua(); 
}

void KillSwitchMonitor :: closeLua()
{
    if (L)
    {
        lua_close(L);
        L = nullptr;
    }
}

bool KillSwitchMonitor :: loadLuaScript(const string &filename)
{
    if (luaL_dofile(L, filename.c_str())!= LUA_OK)
    {
        cout <<"[LUA error]:" << lua_tostring(L, -1) << endl ; 
        lua_pop(L, 1) ;
        return false; 
    }
    return true; 
}

bool KillSwitchMonitor :: monitorLogicIntegrity(const string &logicname, float threatScore)
{
    string decision = queryActionPolicy(threatScore, logicname ); 
    if (decision == "KILL")
    {
        cout <<"KIll switch Abort Logic:" << logicname << endl;
        return true ; 
    }
    else if (decision == "ROLLBACK")
    {
        cout <<"Killswitch - Trigger rollback for:" << logicname << endl; 
        return false; 
    }
    return false; 
}

bool KillSwitchMonitor :: evaluateAbortCondition(float threatScore, const string &source)
{
    return (threatScore >= killThreshold);
}

string KillSwitchMonitor :: queryActionPolicy(float score, const string &source)
{
    lua_getglobal(L, "Evaluate killaction"); 
    lua_pushnumber(L, score);
    lua_pushstring(L, source.c_str()); 

    if (lua_pcall(L, 2,1, 0) != LUA_OK)
    {
        cout <<"Lua error - Failed to call evaluate" << endl; 
        lua_pop(L, 1); 
        return "SAFE";
    }
    string result = lua_tostring(L, -1); 
    lua_pop(L, 1); 
    return result; 
}