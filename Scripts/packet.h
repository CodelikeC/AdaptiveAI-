// packet.h
#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <algorithm> 

using namespace std;

// Cấu trúc dữ liệu cho raw data
struct Packet 
{
    string src_ip;
    string dest_ip;
    string protocol;
    string timestamp;
    int payload_size;

    Packet(string s_ip, string d_ip, string prot, string ts, int size)
        : src_ip(s_ip), dest_ip(d_ip), protocol(prot), timestamp(ts), payload_size(size) {}
};

// Biến toàn cục để lưu lịch sử rủi ro
extern map<string, vector<double>> riskHistory;

// Hàm kiểm tra trạng thái hệ thống
bool isSystemStable() 
{
    bool networkStable = false;
    auto start = chrono::high_resolution_clock::now();
    int pingTime = rand() % 100; 
    // Giả lập ping (ms)
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    if (pingTime < 50) 
    networkStable = true;

    int cpuUsage = rand() % 100; 
    // Giả lập tải CPU
    bool cpuStable = (cpuUsage < 90);

    bool serverActive = (rand() % 100) > 10; 
    // Giả lập server

    bool systemStable = networkStable && cpuStable && serverActive;
    if (!systemStable)
    {
        cout << "Unstable system: Network=" << (networkStable ? "OK" : "Fail")
             << ", CPU=" << cpuUsage << "%, Server=" << (serverActive ? "Active" : "Down") << endl;
    }
    return systemStable;
}

// Hàm lấy thời gian hiện tại
string getCurrentTime() 
{
    time_t now = time(0);
    return ctime(&now);
}

// Hàm cảm biến môi trường
bool detectEnvironmentalChange(const Packet& packet, const map<string, int>& packetCount) 
{
    string key = packet.src_ip + "_" + packet.timestamp.substr(0, 16);
    int count = packetCount.count(key) ? packetCount.at(key) : 0;
    if (packet.payload_size > 1000 || count > 300) 
    {
        return true; 
        // Phát hiện bất thường
    }
    return false;
}

#endif 
// PACKET_H