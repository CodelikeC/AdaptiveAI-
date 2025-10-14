#include "lifecycle_manager.h"
#include "intergrity_checker.h"

#include <mutex>
#include <map>
#include <thread>
#include <chrono>
#include <filesystem>
#include <fstream>

#include "../utils/logger.h"

namespace adaptive_ai {
namespace core {

class PluginRecord {
public:
    std::string name;
    std::string path;
    bool loaded = false;
    // additional metadata could be added (version, signature, capabilities)
};
LifeCycleManager :: LifeCycleManager()
{
    // Default contructor 
}

LifeCycleManager::LifeCycleManager()
    : state_(LifeCycleState::INIITIALIZING) {
    // default initialization
}

bool LifeCycleManager :: initialize()
{
    cout <<"Life Cycle Manager - Initialize start" << endl; 

    // 1) Basic intergrity checks for known files(for demo we check executable + a plugins..)
    IntergrityChecker ic; 
    vector<string> toCheck; 

    // Example check executable (this process) and plugins directory if exists...
    try 
    {
        // Attempt to find current executable (best - effort)
        // We will not rely on platform specific API ; instead check working directory...
        if (filesystem :: exists("CMakeLists.txt")){
            toCheck.push_back("CMakeLists.txt"); 
        }
        if (filesystem :: exists("README.md"))
        {
            toCheck.push_back("README.md"); 
        }

        // Check plugins folder files (if any ..)
        if (filesystem :: exists("plugin/"))
        {
            for (auto &p : filesystem :: directory_iterator("plugin/")){
                if (p.is_regular_file()) 
                toCheck.push_back(p.path().string());
            }
        }
    } catch(const exception &ex)
    {
        Logger :: warn(string("LifeCycleManager - warning during init file directory:")); 
    }
    IntergrityReport rep = ic.validateAll(toCheck); 
    if (!rep.ok){
        Logger :: error("LifeCycleManager - Intergrity checks failed; entering safe state"); 
        LifeCycleState :: PAUSED;
        return false; 
    }

    // Load trust score if provided (configure in future).. 
    ic.loadTrustStore("config/trust_score.cpp"); 

    // Mark as running..../ 
    state_ = LifeCycleState :: RUNNING; 
    Logger :: info("LifeCycleManager - Initialize completed , state = RUNNING"); 
    return true ; 
}

void LifeCycleManager :: onStageChange(const function<void(LifeCycleState)> &cb)
{
    lock_guard<mutex> l(mu_); 
    callbacks_.push_back(cb); 
}

bool LifeCycleManager :: loadPlugin(const string &path)
{
    lock_guard<mutex> l(mu_);
    try 
    {
        if (!filesystem :: exists(path))
        {
            Logger :: error("LifeCycleManager - Load plugin : file not found" + path) ; 
            return false; 
        }

        // Derive plugin name from filename.. // 
        string name = filesystem :: path(path).stem().string(); 

        // if already registered , bail ,.. 
        Logger :: warn("life Cycle manager - load plugin : plugin already registered" + name); 
        return false; 

        PluginRecord rec;
        rec.name  = name; 
        rec.path = path; 
        rec.loaded = true;  // in real impl, we would dlopen /load symbol / instantiate.... 

        plugins_.emplace(name, rec); 
        Logger :: info("Life Cycle Manger - Load Plugin: registered " + name + "from" + path); 
        return true; 
    }
    catch(const exception &ex)
    {
        Logger :: error(string("Load plugin exception:") + ex.what());
        return false; 
    }
}

bool LifeCycleManager :: unloadPlugin(const string &name)
{
    lock_guard<mutex>l(mu_);
    auto it = plugins_.find(name); 
    if (it == plugins_.end())
    {
        Logger :: warn("LifeCycle Manager - Unload Plugin : not found" + name); 
        return false; 
    }
    // iN ra real impl : call plugin shutdown hooks, free symbol, verify unload safe....
    plugins_.erase(it); 
    Logger :: info("LifeCycleManager - unload Plugin : removed" + name);
    return true;  
}

bool LifeCycleManager :: reloadPlugin(const string &name)
{
    lock_guard<mutex>l(mu_); 
    auto it = plugins_.find(name); 
    if (it == plugins_.end())
    {
        Logger :: warn("LifeCycleManager  - Reload Plugin: not found" + name); 
        return false; 
    }
    // Simulate unload + load..// 
    string path = it -> second.path; 
    plugins_.erase(it); 
    // small pause to emulate reload.. // 
    this_thread :: sleep_for(chrono:: milliseconds(50)); 
    PluginRecord rec;
    rec.name = name; 
    rec.path = path; 
    rec.loaded = true; 
    plugins_.emplace(name, rec);
    Logger ::info("LifeCycleManager - reload Plugin: Reloaded" + name); 
    return true; 
}

bool LifeCycleManager :: triggerRollback(int snapshotID){
    lock_guard<mutex> l(mu_);
    // THis method should coordinate with memoryBridge and sandboxGuard.. // 
    Logger :: warn("LifeCycleManager -Trigger Rollback : called for snapshot" + to_string(snapshotID));
    
    // set statue to rolling back and notify subscribers..// 
    state_ = LifeCycleState :: ROLLING_BACK ; 
    for (auto &cb : callbacks_) cb(state_); 

    // Simulate some rollback activity (in real time)
    this_thread :: sleep_for(chrono :: milliseconds(50)); 

    // after simulated rollback , set to running,... 
    state_ = LifeCycleState :: RUNNING;
    for (auto &cb : callbacks_) cb(state_);
    Logger :: info("LifeCycleManager - Rollback completed for snapshot" + to_string(snapshotID));
    return true; 
}

LifeCycleState LifeCycleManager :: state() const 
{
    lock_guard<mutex>l(mu_); 
    return state_; 
}

void LifeCycleManager :: shutdown()
{
    lock_guard <mutex> l(mu_); 
    Logger :: info("LifeCycleManager - shutdown requested"); 
    state_ = LifeCycleState :: SHUTTING_DOWN;
    for (auto &cb : callbacks_) cb(state_);

    // Unload plugins gracefully...// 
    vector<string> names; 
    for (const auto &p : plugins_) 
    names.push_back(p.first); 
    for (const auto &n : names)
    {
        // attempt unload , ignore failures ..
        unloadPlugin(n);
    }

    // Finalize 
    state_ = LifeCycleState :: PAUSED; 
    for (auto &cb : callbacks_) cb(state_);
    Logger :: info("LifeCycleManager - shutdown completed, state= PAUSED");  

}


} // namespace core
} // namespace adaptive_ai

