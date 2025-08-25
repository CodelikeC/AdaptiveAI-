//packet
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;
#include "packet.h"
#include <cstdlib>

// Biến toàn cục định nghĩa
map<string, vector<double>> riskHistory;

class LogicDynamic 
{
private:
    map<string, int> packetCount;      
    // Đếm gói tin trong 1 phút
    map<string, int> ignoreCount;      
    // Đếm lần bỏ qua cảnh báo
    map<string, int> userRejectionCount; 
    // Đếm lần từ chối của user
    double sensitivity;                
    // Độ nhạy (tự điều chỉnh)
    int threshold;                     
    // Ngưỡng cảnh báo (tự tối ưu)
    bool fallbackMode;                 
    // Chế độ dự phòng
    const int MAX_IGNORE = 5;          
    // Ngưỡng tối đa bỏ qua
    const int REJECTION_THRESHOLD = 5; 
    // Ngưỡng tối đa từ chối
    const int DEFAULT_THRESHOLD = 50;  
    // Ngưỡng an toàn
    vector<string> transparencyLog;    
    // Nhật ký minh bạch

    // Nhóm 1: Cảm biến môi trường
    void updatePacketCount(const Packet& packet) 
    {
        string key = packet.src_ip + "_" + packet.timestamp.substr(0, 16);
        packetCount[key]++;
    }

    bool detectEnvironmentalChange(const Packet& packet) 
    {
        return ::detectEnvironmentalChange(packet, packetCount);
    }

    // Nhóm 2: Phân tích rủi ro
    double calculateRiskScore(const Packet& packet) 
    {
        string key = packet.src_ip + "_" + packet.timestamp.substr(0, 16);
        int count = packetCount[key];
        double score = 0.0;

        if (packet.protocol == "UDP") 
        score += 0.3;

        if (count > 40) 
        score += 0.5;

        if (packet.payload_size > 1000) 
        score += 0.2;

        return score;
    }

    // Nhóm 3: Học hỏi
    void learnFromHistory(const string& key, double riskScore) 
    {
        riskHistory[key].push_back(riskScore);
        if (riskHistory[key].size() > 10) riskHistory[key].erase(riskHistory[key].begin());

        double avgRisk = 0.0;
        for (double r : riskHistory[key]) 
        avgRisk += r;
        avgRisk /= riskHistory[key].size();

        if (avgRisk > 0.7 && sensitivity < 1.0) 
        sensitivity += 0.05;
        else if (avgRisk < 0.3 && sensitivity > 0.2)
        sensitivity -= 0.05;
    }


    // Nhóm 4: Thực thi
    void executeAdaptation(const string& key, int count) 
    {
        if (count > threshold * sensitivity && !fallbackMode) 
        {
            threshold = max(100, static_cast<int>(threshold * 1.5));
            logAdaptation("Change to warning mode");
            cout << "Activate the priority mode: new levels " << threshold << " packet/60s." << endl;
        } 
        else if (count < threshold * 0.5 && !fallbackMode) 
        {
            threshold = max(30, static_cast<int>(threshold/ 1.5));
            logAdaptation("Out of the warning modes");
            cout << "Out of the warning mode: New levels" << threshold << " packet/60s." << endl;
        }
    }

    // Nhóm 5: HITL
    string proposeAction(const string& key, int count) 
    {
        double adjustedThreshold = threshold * sensitivity;
        if (count > adjustedThreshold) 
        {
            return "Recommended: Increase sensitivity to " + to_string(sensitivity + 0.1) +
                   " and send warning. Agree? [Yes/No]";
        }
        return "No action.";
    }

    void monitorUserBehavior(const string& user_id, const string& response) 
    {
        if (response == "No") 
        {
            userRejectionCount[user_id]++;
            if (userRejectionCount[user_id] >= REJECTION_THRESHOLD) 
            {
                cout << "Warning: User " << user_id << " denied too many times. Notify admin." << endl;
                userRejectionCount[user_id] = 0;
            }
        }
    }

    void crossValidate(const Packet& packet, int count, const string& response, const string& user_id) 
    {
        double riskScore = calculateRiskScore(packet);
        if (riskScore > 0.7 && response == "No") 
        {
            cout << "Warning: High risk (" << riskScore << ") but user " << user_id << " denied. Notify admin." << endl;
        }
    }

    void adjustLogic(const string& key, const string& response, const string& user_id) 
    {
        string action = proposeAction(key, packetCount[key]);
        logHITLAction(user_id, action, response);
        if (response == "No") 
        {
            ignoreCount[key]++;
            if (ignoreCount[key] >= MAX_IGNORE) 
            {
                threshold += 10;
                ignoreCount[key] = 0;
                cout << "Dynamic Logic: Increased threshold to " << threshold << " packets/60s. Accept? [Yes/No]" << endl;
            }
        } 
        else if (response == "Yes") 
        {
            sensitivity = min(sensitivity + 0.1, 1.0);
            ignoreCount[key] = 0;
        }
    }

    void enterFallbackMode(const string& user_id) 
    {
        fallbackMode = true;
        threshold = DEFAULT_THRESHOLD;
        logAdaptation("Enter fallback mode due to excessive rejections from " + user_id);
        cout << "Entered automatic mode: Excessive rejections from " << user_id << ". Notify admin." << endl;
    }

    void logHITLAction(const string& user_id, const string& action, const string& response) 
    {
        string logEntry = "User: " + user_id + ", Action: " + action + ", Response: " + response + ", Time: " + getCurrentTime();
        transparencyLog.push_back(logEntry);
        sendLogToServer(logEntry);
    }

    void sendLogToServer(const string& logEntry) 
    {
        cout << "Sent to server: " << logEntry << endl;
    }

    // Nhóm 6: Tự sửa
    void selfHeal() 
    {
        if (!isSystemStable() && !fallbackMode) 
        {
            fallbackMode = true;
            threshold = DEFAULT_THRESHOLD;
            logAdaptation("Change to plan B");
            cout << "self - heal : change to plan B with levels: " << threshold << "." << endl;
        } 
        else if (isSystemStable() && fallbackMode) 
        {
            fallbackMode = false;
            logAdaptation("Recover stable mode");
            cout << "Recover: Stable mode." << endl;
        }
    }

    // Nhóm 7: Ghi log
    void logAdaptation(const string& action) 
    {
        string logEntry = "Action: " + action + ", Time: " + getCurrentTime();
        transparencyLog.push_back(logEntry);
        cout << "write log: " << logEntry << endl;
    }

public:
    void processRawData(const Packet& packet, const string& user_id) 
    {
        // Bước 1: Cập nhật dữ liệu
        updatePacketCount(packet);

        // Bước 2: Cảm biến môi trường
        if (detectEnvironmentalChange(packet)) 
        {
            selfHeal();
        }

        // Bước 3: Phân tích rủi ro
        string key = packet.src_ip + "_" + packet.timestamp.substr(0, 16);
        int count = packetCount[key];
        double riskScore = calculateRiskScore(packet);

        // Bước 4: Học hỏi
        learnFromHistory(key, riskScore);

        // Bước 5: Thực thi
        executeAdaptation(key, count);

        // Bước 6: HITL
        if (!fallbackMode) 
        {
            string action = proposeAction(key, count);
            if (action != "No action.") 
            {
                cout << action << endl;
                string response;
                cin >> response;
                monitorUserBehavior(user_id, response);
                crossValidate(packet, count, response, user_id);
                if (userRejectionCount[user_id] >= REJECTION_THRESHOLD) 
                {
                    enterFallbackMode(user_id);
                } 
                else 
                {
                    adjustLogic(key, response, user_id);
                }
            }
        }

        // Bước 7: Ghi log và hiển thị
        logAdaptation("Processed packet from " + packet.src_ip);
        cout << "Data processing: src_ip " << packet.src_ip << " - " << count
             << " packet/60s, Risk: " << fixed << setprecision(2) << riskScore
             << ", Sensitivity: " << sensitivity << endl;

        // Bước 8: Chế độ dự phòng
        if (fallbackMode && count > threshold) 
        {
            cout << "Warning (auto mode): src_ip " << packet.src_ip << " out the line " << threshold << "." << endl;
        }
    }

    LogicDynamic() : sensitivity(0.5), threshold(30), fallbackMode(false) {}
};

int main() 
{
    LogicDynamic safeAI;

    // Dữ liệu giả lập
    vector<Packet> rawData = 
    {
        Packet("192.168.1.10", "203.0.113.5", "UDP", "2025-05-03 01:00:00", 512),
        Packet("192.168.1.10", "203.0.113.5", "UDP", "2025-05-03 01:00:10", 1200),
        Packet("192.168.1.11", "203.0.113.6", "TCP", "2025-05-03 01:01:00", 300),
        Packet("192.168.1.10", "203.0.113.5", "UDP", "2025-05-03 01:01:10", 1500),
    };

    string user_id = "user_1";
    for (const Packet& packet : rawData) 
    {
        safeAI.processRawData(packet, user_id);
    }

    return 0;
}