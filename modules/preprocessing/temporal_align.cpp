#include "temporal_align.h"
#include <algorithm>
#include <iostream>

TemporalAlign::TemporalAlign() {}

TemporalAlign::~TemporalAlign() {}

void TemporalAlign::addEvent(const string& source, const string& data) 
{
    TimedEvent e;
    e.timestamp = chrono::system_clock::now();
    e.source = source;
    e.data = data;
    events.push_back(e);
}

vector<TimedEvent> TemporalAlign::getChronoLogicalEvent() 
{
    vector<TimedEvent> sorted_events = events;
    sort(sorted_events.begin(), sorted_events.end(), compareByTime);
    return sorted_events;
}

bool TemporalAlign::compareByTime(const TimedEvent& a, const TimedEvent& b) 
{
    return a.timestamp < b.timestamp;
}

map<int, vector<TimedEvent>> TemporalAlign::getWindowEvent(int window_seconds) 
{
    map<int, vector<TimedEvent>> windows;
    auto sorted = getChronoLogicalEvent();

    for (const auto& e : sorted) 
    {
        auto epoch = chrono::duration_cast<chrono::seconds>(e.timestamp.time_since_epoch()).count();
        int window_id = epoch / window_seconds;
        windows[window_id].push_back(e);
    }

    return windows;
}


