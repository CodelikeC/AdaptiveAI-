#include <iostream>
using namespace std; 
#include <string> 
#include <vector> 

#include "plugin_loader.h"
#include "sensing_plugin.h"

namespace adaptive_ai
{
    SensingPlugin :: SensingPlugin() : isIntialized(false){}

    SensingPlugin :: ~SensingPlugin()
    {
        shutdown(); // Dam bao cleanup khi huy .. // 
    }

    string SensingPlugin :: name() const 
    {
        return "SensingPlugin"; 
    }

    void SensingPlugin :: initialize()
    {
        if (!isIntialized)
        {
            cout <<"Sensing Plugin intializing..." << endl; 
            sensedData.clear(); // muon khoi tao lai du lieu
            isIntialized = true; 
        }
    }

    void SensingPlugin :: execute()
    {
        if (isIntialized)
        {
            cout <<"Sensing Plugin executing..." << endl; 
            // vi du : thu thap du lieu gia lap.. . 
            sensedData.push_back("CPU : 50 %"); 
            for (const auto &data : sensedData)
            {
                cout <<"Sensed:" << data << endl; 
            }
        }
        else 
        {
            cout <<"Sensing plugin not initialized" << endl;
        }
    }

    void SensingPlugin :: shutdown()
    {
        if (isIntialized)
        {
            cout <<"Sensing plugin shutting down" << endl; 
            sensedData.clear(); 
            isIntialized = false; 
        }
    }

    void PluginManager :: load_plugins(const string *directory)
    {
        cout <<"Load plugins" << endl; 
        vector<Plugin*> plugins; 
        cout <<"-----" << endl;
        // Giả định load Sensing plugins nhu mot instance.. ..
        SensingPlugin *plugin;  
        plugins.push_back(plugin);
        handles.push_back(nullptr); 
        
        plugin -> intialize(); 

        
    }

    void PluginManager :: execute_all()
    {
        cout <<"Execute all" << endl;
        cout <<"-----------" << endl; 
        vector<void*> handles;
        for (auto *plugin : plugins)
        {
            plugin -> execute(); 
        }
    }

    void PluginManager :: unload_all()
    {
        cout <<"Unload all" << endl; 
        for (auto *plugin : plugins)
        {
            plugin -> shutdown(); 
            delete plugin;
        }
        plugins.clear();
    }
}
