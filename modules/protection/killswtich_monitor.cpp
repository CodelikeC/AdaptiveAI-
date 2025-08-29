#include "killswitch_monitor.h"
#include <iostream>

extern "C" 
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include <string>

using namespace std;

KillSwitchMonitor::KillSwitchMonitor() {
    initLua();
}

KillSwitchMonitor::~KillSwitchMonitor() {
    shutdown();
}

void KillSwitchMonitor::initLua() {
    if (!L) {
        L = luaL_newstate();
        if (!L) {
            cout << "[ERROR] Failed to create new Lua state\n";
            return;
        }
        luaL_openlibs(L);
        // only attempt to load if state is valid
        loadLuaScript("Plugin/lua/killswitchNewLogic.lua");
    }
}

void KillSwitchMonitor::shutdown() {
    closeLua();
}

void KillSwitchMonitor::closeLua() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

void KillSwitchMonitor::initialize() {
    if (!L) {
        initLua(); // ensure state is created
    }
}

bool KillSwitchMonitor::loadLuaScript(const string &filename) 
{
    if (!L) 
    {
        cout << "[LUA error]: Lua state is null\n";
        return false;
    }

    if (luaL_dofile(L, filename.c_str()) != LUA_OK) 
    {
        const char* msg = lua_tostring(L, -1);
        cout << "[LUA error]:" << (msg ? msg : "unknown") << endl;
        lua_pop(L, 1);
        return false;
    }
    
    return true;
}

bool KillSwitchMonitor :: monitorLogicIntergrity(const string &logicname, float threatScore)
{
    string decision = queryActionPolicy(threatScore, logicname);
    if (decision == "KILL") 
    {
        cout << "Kill switch Abort Logic: " << logicname << endl;
        return true;
    } 
    else if (decision == "ROLLBACK") 
    {
        cout << "Killswitch - Trigger rollback for: " << logicname << endl;
        return false;
    }
    return false;
}

bool KillSwitchMonitor::evaluateAbortCondition(float threatScore, const string &source) 
{
    return (threatScore >= killThresHold);
}

string KillSwitchMonitor::queryActionPolicy(float score, const string &source) {
    if (!L) 
    {
        cout << "Lua state is null in queryActionPolicy\n";
        return "SAFE";
    }

    // IMPORTANT: the function name in Lua cannot contain spaces.
    // ensure your Lua script defines e.g. function evaluate_killaction(score, source) ... end
    lua_getglobal(L, "evaluate_killaction"); // use a valid identifier
    if (!lua_isfunction(L, -1)) 
    {
        cout << "Lua function 'evaluate_killaction' not found\n";
        lua_pop(L, 1);
        return "SAFE";
    }

    lua_pushnumber(L, score);
    lua_pushstring(L, source.c_str());

    if (lua_pcall(L, 2, 1, 0) != LUA_OK) 
    {
        const char* msg = lua_tostring(L, -1);
        cout << "Lua error - Failed to call evaluate_killaction: " << (msg ? msg : "unknown") << endl;
        lua_pop(L, 1);
        return "SAFE";
    }

    const char* res = lua_tostring(L, -1);
    string result = res ? res : "SAFE";
    lua_pop(L, 1);
    return result;
}
