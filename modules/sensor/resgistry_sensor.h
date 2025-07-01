#ifndef registry_sensor_h 
#define registry_sensor_h 

using namespace std; 

#include <windows.h>
#include <iostream>
#include <string> 

#include <algorithm>
#include <thread> 
#include <atomic> 

#include <functional> 

#pragma comment(lib, "ws2_32.lib")

class RegistrySensor
{
    public : 
    RegistrySensor(); 
    ~RegistrySensor(); 

    // khoi dong va dung giam sat registry ..// 
    void startMonitoring(); 
    void stopMonitoring(); 

    // Gan callback khi co su kien..// 
    void setOnRegistryChange(function<void(const string &key)> callback);

    private: 
    thread monitorThread; 
    atomic<bool> isMonitoring; 
    function<void(const string &key) > onchangeCallback;

    // Ham noi bo chay trong giam sat..// 
    void monitor();

    // cau hinh key can giam sat (vi du SOFTWARE HOAC SYSTEM)
    HKEY rootKey = HKEY_CURRENT_USER; 
    wstring subKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
}; 

#endif 