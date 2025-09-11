// oultier_sensor.h 
#pragma once 

#include <functional>
#include <thread> 

#include <atomic>
#include <string> 

class OutlierSensor
{
    public: 
    OutlierSensor();
    ~OutlierSensor(); 

    void startMonitoring(); 
    void stopMonitoring(); 
    void setOnOutlierDetected( std ::function<void(const  std ::string &)>); 

    private: 
    atomic <bool> running; 
    thread monitorThread; 
    function<void(const string&)> onOutlierDetected; 
    void monitor(); 
};
