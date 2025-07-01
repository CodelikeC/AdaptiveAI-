#pragma once 

#ifndef user_behavior_sensor_h 
#define user_behavior_sensor_h 

#include <iostream>
#include <string> 
#include <algorithm>
#include <thread>

#include <chrono>
#include <atomic>
#include <mutex> 
#include <functional> 

using namespace std; 

class UserBehaviorSensor 
{
    public: 
    UserBehaviorSensor(); 
    ~UserBehaviorSensor(); 

    void startMonitoring();
    void stopMonitoring(); 

    void setOnBehaviorDetected(function<void(const string&)> callback); 

    private: 
    atomic <bool> running; 
    thread monitorThread; 
    function <void(const string&)> onBehaviorDetected;

    chrono :: steady_clock::time_point lastMouseEvent ; 
    chrono :: steady_clock::time_point lastKeyEvent; 

    void monitor(); 
    bool detectAnomalousMouse();
    bool detectRepetitiveKeys();
}; 

#endif 