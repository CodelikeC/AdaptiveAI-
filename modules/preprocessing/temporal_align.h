#ifndef temporal_align_h 
#define temporal_align_h 

using namespace std; 
#include <iostream>
#include <string> 

#include <chrono> 
#include <map> 

struct TimedEvent
{
    chrono :: system_clock::time_point timestamp; 
    string source;
    string data;
}; 

class TemporalAlign
{
    public: 
    TemporalAlign(); 
    ~TemporalAlign();

    // Them mot su kien moi ..//
    void addEvent(const string &source, const string &data ); 

    // Tra ve chuoi su kien duoc sap xep theo thoi gian .. // 
    vector<TimedEvent> getChronoLogicalEvent();

    // tao khung thoi gian (timewindow) de gon nhom.. 
    map<int, vector<TimedEvent>> getWindowEvent(int window_seconds); 

    private: 
    vector<TimedEvent> events;

    static bool compareByTime(const TimedEvent &a, const TimedEvent &b); 
}; 
#endif 