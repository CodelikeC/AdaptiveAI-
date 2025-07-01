#include "resgistry_sensor.h"
#include <string> 

using namespace std;

RegistrySensor ::RegistrySensor(): isMonitoring(false){}

RegistrySensor :: ~RegistrySensor()
{
    stopMonitoring();
}

void RegistrySensor :: setOnRegistryChange(function <void(const string &key)> callback)
{
    onchangeCallback = callback;
}

void RegistrySensor :: startMonitoring()
{
    if (isMonitoring) return ; 
    isMonitoring = true; 
    monitorThread = thread(&RegistrySensor :: monitor, this);
}

void RegistrySensor :: stopMonitoring()
{
    isMonitoring = false; 
    if (monitorThread.joinable())
    monitorThread.join(); 
}

void RegistrySensor :: monitor()
{
    HKEY hKey; 
    if (RegOpenKeyExW(rootKey, subKey.c_str(), 0, KEY_NOTIFY, &hKey) != ERROR_SUCCESS)
    {
        cout <<"Failed to open registry key for monitoring" << endl; 
        return ;    
    }

    while (isMonitoring)
    {
        if (RegNotifyChangeKeyValue(
                hKey,
                TRUE,
                REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_LAST_SET,
                NULL,
                FALSE) == ERROR_SUCCESS) {

            if (onchangeCallback) 
            {
                string keyStr(subKey.begin(), subKey.end());
                onchangeCallback(keyStr);
            }
        } 
        else 
        {
            cout << "Registry monitoring error.\n";
            break;
        }
        Sleep(1000);
    }

    RegCloseKey(hKey); 
}