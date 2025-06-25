#include "sandbox.h"

#include <stdio.h>

bool sandbox_plugin(const PluginInfo *plugin)
{
    printf("[SANDBOX] Executing plugin '%s'", plugin ->author); 
    // simulate sandbox .. // 
    return true;
}

void restrict_plugin_capabilities(const PluginInfo *plugin)
{
    printf("Sandbox Restricting capabilities"); 
    // trong thuc te : gioi han syscalls, resource limit, chroot, v.v..// 

}

