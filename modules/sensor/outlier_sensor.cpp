#include "outlier_sensor.h"
#include "../utils/logger.h"
#include <chrono>

using namespace adaptive_ai; 
OutlierSensor :: OutlierSensor() : running(false){}
OutlierSensor :: ~OutlierSensor() { 
    stopMonitoring(); 
}

void OutlierSensor :: startMonitoring()
{
    if (running) return ; 
    running = true; 
    monitorThread = std ::thread(&OutlierSensor :: monitor, this);  
}

void OutlierSensor :: stopMonitoring()
{
    running = false; 
    if (monitorThread.joinable()) 
    monitorThread.join(); 
}

void OutlierSensor :: setOnOutlierDetected(std ::function<void(const std ::string &)> callback)
{
    onOutlierDetected = callback; 
}

void OutlierSensor :: monitor()
{
    while (running)
    {
        // placeholder logic for outlier detection..// 
        Logger::debug("Monitoring for outliers..."); 
        if (onOutlierDetected) 
        onOutlierDetected("Outlier detected (plugin)"); 
        std :: this_thread:: sleep_for(std :: chrono :: seconds(5)); 
    }
}
