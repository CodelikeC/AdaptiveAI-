#include <iostream>
#include <thread>
#include <chrono>

// Sensor includes
#include "network_sensor.h"
#include "system_monitor.h"
#include "file_access_sensor.h"
#include "outlier_sensor.h"
#include "resgistry_sensor.h"
#include "user_behavior_sensor.h"

using namespace adaptive_ai; 

void logAlert(const string &source, const string &message)
{
    cout <<"[" << source <<"] Alert" << message << endl;  
}

int main ()
{
    cout <<"=== Starting Adaptive AI Sensing Demo ====" << endl; 

    // -------------- FILE ACCESS SENSOR ---------------------// 
    File_access_sensor fileSensor;
    
    return 0;
}