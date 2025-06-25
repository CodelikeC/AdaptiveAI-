#ifndef Script_with_lua_h
#define Script_with_lua_h

#include <iostream> 
#include <string> 
#include <concepts>
#include <thread>
#include <lua.h>

using namespace std; 

class UpdateLogic
{
    public: 
    UpdateLogic();
    ~UpdateLogic(); 

    protected: 
    int trust_score; 
    float update_score; 
    bool logic;
    
    
    public:
    // Core logic adaptation function..// 
    int adaptTrustScore(); 
    float adaptupdateScore(); 

    // adaption with Lua scripts 
    bool loadLuaScript(const string &script);
    bool callluaFunction(const string &function);

    private: 
    lua_State *L;
    void initlua(lua_State **L);
    void closeLua();

}; 

#endif 
