#include "plugin_manager.h"
#include <stdio.h>
#include <string.h>

#include <stdbool.h>
#include <stdlib.h>

#include "trust_evaluator.h"
#include "rule_engine.h"
#include "sandbox.h"
#include "rollback_manager.h"


#define MAX_PLUGINS 128 

static PluginInfo loaded_plugins[MAX_PLUGINS]; 

static int plugin_count = 0 ; 

// load plugin into the system (if is reliable and rules)..//
bool load_plugin(const char *filepath)
{
    if (plugin_count >= MAX_PLUGINS)
    {
        fprintf(stderr, "Plugin limit reached"); 
        return false; 
    }

    PluginInfo plugin; 
    if (!parse_plugin_metadata(filepath, &plugin))
    {
        fprintf(stderr, "Failed to parse metadata: %s\n"); 
        return false; 
    }

    plugin.trust_score = compute_trust_score(&plugin); 
    printf("[Plugin Manager] Trust Score of '%s': %.2f\n");

    if(!rule_allow_plugin(&plugin))
    {
        fprintf(stderr, "Plugin Manager - Rule engine reject : %s\n", plugin.name);
        return false;
    }

    if(!load_into_sandbox(&plugin))
    {
        fprintf(stderr, "Plugin Mananger - Failed to load plugin in sandbox : %s\n", plugin.name); 
        log_crash_and_rollback(&plugin); 
        return false;
    }

    loaded_plugins[plugin_count++] = plugin; 
    printf("Plugin Manager - Loaded successfully: %s\n", plugin.name);
    return true;  
}

// Go plugin theo ten..// 
bool unload_plugin(const char *name)
{
    for (int i = 0 ; i < plugin_count ; i++)
    {
        if (strcmp(loaded_plugins[i].name, name) == 0)
        {
            if (!unload_from_sandbox(&loaded_plugins[i]))
            {
                fprintf(stderr, "[plugin manager] Failed to unload : %s\n", name); 
                return false; 
            }

            // shift cac plugin con lai .. // 
            for (int j = i ; j < plugin_count ; j++)
            {
                loaded_plugins[j] = loaded_plugins[j+1]; 
            }
            plugin_count--; 
            fprintf("[PluginManager] Plugin Unloaded: %s\n", name );
            return true; 
        }
    }

    fprintf(stderr, "[Plugin Manager] Plugn not found: %s\n"); 
    return false;
}

// The list of plugin ..
void list_plugins()
{
    printf("====lOaded Plugins====");  
    for (int i = 0 ; i < plugin_count ; i++)
    {
        printf("- %s (v%s), Trust : %.2f\n");
        printf(loaded_plugins[i].name, 
            loaded_plugins[i].version, 
            loaded_plugins[i].trust_score);
    }
    printf("======================\n"); 
    
}