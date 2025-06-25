#include <iostream>
using namespace std; 

#include <algorithm>
#include <cmath> 

#include "ScriptwithLua.h"
#include <vector> 
#include <lua.h>

UpdateLogic :: UpdateLogic()
{
    trust_score = 0; 
    update_score = 0 ; 
    logic = false; 
    
}

UpdateLogic :: ~UpdateLogic()
{
    closeLua(); 
}

lua_State *L = NULL;

void UpdateLogic :: initlua(lua_State **L)
{
    // *L = luaL_newstate();
    // luaL_openlibs(L);
}

void UpdateLogic :: closeLua()
{
    if (L) lua_close(L);
}

bool UpdateLogic :: loadLuaScript(const string &scriptName)
{   
    
}

bool UpdateLogic :: callluaFunction(const string &functionName)
{
    lua_getglobal(L, functionName.c_str());
    if (!lua_isfunction(L , - 1))
    {
        cout <<"Lua function" << functionName << endl; 
        return false; 
    }
    if (lua_pcall(L, 0, 1 , 0) != LUA_OK)
    {
        cout <<"Error calling lua function" << endl; 
        cout <<"Lua error:" << lua_tostring(L, -1) << endl;
        return false;  
    }
    if (lua_isnumber(L, -1))
    {
        trust_score = (int)lua_tointeger(L, -1) ;
        lua_pop(L,1) ; 
        return true;
    }
    return false;
}

int UpdateLogic :: adaptTrustScore()
{
    loadLuaScript("_Check_the_logic.lua");
    callluaFunction("check_trust");
    return trust_score;  
}

float UpdateLogic :: adaptupdateScore()
{
    loadLuaScript("Update_new_logic.lua"); 
    lua_getglobal(L, "update_score");
    if (lua_pcall(L, 0,1, 0) == LUA_OK and lua_isnumber(L, -1))
    {
        update_score = (float) lua_tonumber(L, -1);
        lua_pop(L,1);
    }
    return update_score;
}
