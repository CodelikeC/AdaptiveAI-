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
    FileAccessSensor fileSensor;
    fileSensor.setOnSuspiciousAccess([](const std::string& path) {
        logAlert("FileAccessSensor", "Suspicious file accessed: " + path);
    });
    fileSensor.watchFile("C:\\sensitive_data\\secrets.txt");

    // ------------------ SYSTEM MONITOR ------------------
    SystemMonitor sysMonitor;
    sysMonitor.setOnResourceSpike([](const std::string& metric, float value) {
        logAlert("SystemMonitor", metric + " spike detected: " + std::to_string(value));
    });

    // ------------------ NETWORK SENSOR ------------------
    NetworkSensor netSensor;
    netSensor.setOnAnomalyDetected([](const std::string& ip) {
        logAlert("NetworkSensor", "Anomaly from IP: " + ip);
    });

    // ------------------ OUTLIER SENSOR ------------------
    OutlierSensor outlierSensor;
    outlierSensor.setOnOutlierDetected([](const std::string& signal) {
        logAlert("OutlierSensor", "Unusual behavior: " + signal);
    });
    outlierSensor.feed("login_fail");
    outlierSensor.feed("login_fail");
    outlierSensor.feed("login_fail"); // Trigger threshold

    // ------------------ REGISTRY SENSOR ------------------
    RegistrySensor regSensor;
    regSensor.setOnUnauthorizedWrite([](const std::string& key) {
        logAlert("RegistrySensor", "Unauthorized registry change: " + key);
    });

    // ------------------ USER BEHAVIOR SENSOR ------------------
    UserBehaviorSensor userSensor;
    userSensor.setOnBehaviorDetected([](const std::string& behavior) {
        logAlert("UserBehaviorSensor", behavior);
    });
    userSensor.startMonitoring();

    // Simulate sensing for 30 seconds
    for (int i = 0; i < 30; ++i) {
        fileSensor.checkAccess();
        sysMonitor.checkResources();
        netSensor.checkTraffic();
        regSensor.checkRegistry();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    userSensor.stopMonitoring();

    std::cout << "=== Sensing Complete ===\n";
    
    return 0;
}
