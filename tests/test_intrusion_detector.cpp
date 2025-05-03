#include <iostream> 
using namespace std; 

#include <algorithm> 

#include "../modules/protection/intrusion_detector.h"
#include "../modules/sensor/network_sensor.h"
#include <string> 

#include <map> 
#include <vector> 

using namespace adaptive_ai; 

int main ()
{
    cout <<"==============================" << endl; 
    cout <<"======Intrusion Dectector=====" << endl;
    cout <<"==============================" << endl; 

    // khoi tao object cho intrusion detector ..////
    IntrusionDetector detector; 

    //Tạo dữ liệu mô phỏng traffic mạng
    vector<NetworkSensor::packet_info> packet_buffer = 
    {
    {"192.168.1.10", "10.0.0.2", 443, 8080, "TCP", 1500, 1670000010},
    {"192.168.1.10", "10.0.0.2", 443, 8080, "TCP", 1600, 1670000020},
    {"192.168.1.10", "10.0.0.2", 443, 8080, "TCP", 1700, 1670000030},
    // Mô phỏng một gói bất thường
    {"192.168.1.10", "8.8.8.8", 12345, 53, "UDP", 8000, 1670000040}
    };

    // MÔ phỏng dữ liệu đầu vào nghi ngờ..// 
    vector<map<string, string>> test_inputs = 
    {
        { {"user", "alice"}, {"action", "login"}, {"ip", "192.168.1.10"} },
        { {"user", "bob"}, {"action", "delete_all"}, {"ip", "10.0.0.1"} },
        { {"user", "admin"}, {"action", "upload"}, {"content", "DROP TABLE users;"} },
        { {"user", "guest"}, {"action", "view"}, {"ip", "8.8.8.8"} }
    };

    cout <<"Starting intrusion_detector" << endl; 

    // tao bộ đếm cho mỗi lần dữ liệu độc hại
    int suspicious_count = 0; 
    for (const auto &input : test_inputs)
    {
        if (detector.detect_intrusion(input))
        {
            cout <<"Suspicious activity detected" << endl; 
            suspicious_count++ ; 
        }
        else 
        {
            cout <<"[-] Activity appears normal " << endl; 
        }
    }

    // in ra cac mau da duoc phat hien .. //
    const auto &patterns  = detector.get_detected_patterns();
    
    cout <<"All patterns has been detected" << endl; 
    for (const auto &pattern : patterns)
    {
        cout << "Pattern:" << pattern << endl;
    }

    cout <<"Suspicous event detected:" << suspicious_count << endl;
    return 0 ; 
}
