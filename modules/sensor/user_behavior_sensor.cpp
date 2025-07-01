#include "user_behavior_sensor.h"
#include <iostream>

#include <windows.h> // For mouse and keyboard hooks ..// 
#include <cmath> 

UserBehaviorSensor :: UserBehaviorSensor() : running(false){}

UserBehaviorSensor :: ~UserBehaviorSensor()
{
    stopMonitoring(); 
}

void UserBehaviorSensor :: setOnBehaviorDetected(function<void(const string &) >callback)
{
    onBehaviorDetected = callback;
}

void UserBehaviorSensor :: startMonitoring()
{
    running = true ; 
    monitorThread = thread(&UserBehaviorSensor::monitor, this); 

}

void UserBehaviorSensor :: stopMonitoring()
{
    running = false; 
    if (monitorThread.joinable())
    {
        monitorThread.join(); 
    }
}

void UserBehaviorSensor :: monitor()
{
    while (running)
    {
        bool mouseAnomaly = detectAnomalousMouse(); 
        bool keyAnomaly = detectRepetitiveKeys(); 

        if (mouseAnomaly && onBehaviorDetected)
        {
            onBehaviorDetected("Anomalous mouse movement detected");
        }
        if (keyAnomaly && onBehaviorDetected)
        {
            onBehaviorDetected("Repititive key pattern detected"); 
        }

        this_thread :: sleep_for(chrono :: seconds(2)); 
    }
}

bool UserBehaviorSensor :: detectAnomalousMouse()
{
    POINT pt; 
    GetCursorPos(&pt);
    static POINT lastPt = pt; 

    int dx = abs(pt.x - lastPt.x); 
    int dy = abs(pt.y - lastPt.y); 

    lastPt = pt; 

    if ((dx > 500 || dy > 500) && (dx < 10000 && dy < 10000))
    {
        return true; 
    }
    return false; 
}

bool UserBehaviorSensor :: detectRepetitiveKeys()
{
    static int repeatCounter = 0 ; 
    static SHORT lastKey = 0; 

    for (SHORT key = 0x88 ; key <= 0xFE; ++key)
    {
        SHORT state  = GetAsyncKeyState(key);
        if (state & 0x8000)
        {
            if (key == lastKey)
            {
                repeatCounter++; 
            }
            else 
            {
                repeatCounter = 0 ; 
            }
            lastKey = key; 
            if (repeatCounter >= 20)
            {
                repeatCounter = 0 ; 
                return true; 
            }
        }
    }
    return false ; 
}