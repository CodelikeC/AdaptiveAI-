#ifndef file_access_sensor_h 
#define file_access_sensor_h 

#include <windows.h>
#include <iostream>

#include <string> 
#include <vector> 

#include <thread> 
using namespace std;

#include <atomic> 
#include <functional> 

class File_access_sensor
{
    public: 
    File_access_sensor(const wstring &directoryToMonitor); 
    ~File_access_sensor(); 

    // bat dau / dung giam sat ..
    void startMonitoring(); 
    void stopMonitoring(); 

    // callback khi phat hien truy cap file..// 
    void setOnFileAccess(function <void(const wstring &path, const wstring &action)>setonFile);

    private: 
    void monitor(); // luong giam sat noi bo....

    wstring directory; 
    atomic<bool> monitoring; 
    thread monitorThread;

    HANDLE hdir;
    function<void(const wstring &, const wstring&) >onAccess;
}; 

#endif 