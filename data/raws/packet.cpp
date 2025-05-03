#include <iostream> 
using namespace std; 
#include <map> 
#include <string>

// Định nghĩa cấu trúc dữ liệu cho raw data
struct Packet 
{
    string src_ip;         // Địa chỉ IP nguồn
    string dest_ip;        // Địa chỉ IP đích
    string protocol;       // Giao thức (UDP, TCP, v.v.)
    string timestamp;      // Thời gian
    int payload_size;      // Kích thước gói tin

    // Hàm khởi tạo
    Packet(string s_ip, string d_ip, string prot, string ts, int size) 
        : src_ip(s_ip), dest_ip(d_ip), protocol(prot), timestamp(ts), payload_size(size) {}
};

// Lớp LogicDynamic đại diện cho logic động
class LogicDynamic 
{
private:
    map<string, int> packetCount;      
    // Đếm số gói tin từ mỗi src_ip trong 1 phút
    map<string, int> ignoreCount;      
    // Đếm số lần bỏ qua cảnh báo cho mỗi src_ip
    map<string, int> userRejectionCount; 
    // Đếm số lần từ chối của mỗi user
    double sensitivity;                
    // Độ nhạy (0.0 đến 1.0)
    int threshold;                     
    // Ngưỡng cảnh báo (gói/phút)
    bool fallbackMode;                 
    // Chế độ dự phòng
    const int MAX_IGNORE = 5;          
    // Ngưỡng tối đa bỏ qua để điều chỉnh logic
    const int REJECTION_THRESHOLD = 5; 
    // Ngưỡng tối đa từ chối để cảnh báo
    const int DEFAULT_THRESHOLD = 50;  
    // Ngưỡng an toàn mặc định trong chế độ dự phòng
    vector<string> transparencyLog;    
    // Nhật ký minh bạch

    // co thoi gian thuc .. 
    string getCurrentTime() 
    {
        time_t now = time(0);
        return ctime(&now); // Định dạng: Day Mon DD HH:MM:SS YYYY
    }


    // Hàm cập nhật số gói tin từ raw data
    void updatePacketCount(Packet packet) 
    {
        string key = packet.src_ip + "_" + packet.timestamp.substr(0, 16); // Ghép src_ip và phút
        packetCount[key]++;  // Tăng số gói tin trong phút hiện tại
    }

    // Hàm đề xuất hành động dựa trên ngưỡng
    string proposeAction(const string& key, int count) 
    {
        double adjustedThreshold = threshold * sensitivity;
        if (count > adjustedThreshold) 
        {
            return "Recommended: Up the sensor " + to_string(sensitivity + 0.1) +
                   " and send the warning. Agree? [Yes/No]";
        }
        return "No action.";
    }

    // Hàm điều chỉnh logic dựa trên phản hồi HITL
    void adjustLogic(string key, string response) {
        if (response == "Không") {
            ignoreCount[key]++;
            if (ignoreCount[key] >= MAX_IGNORE) {
                threshold += 10;  // Tăng ngưỡng nếu bỏ qua quá nhiều
                ignoreCount[key] = 0;  // Reset đếm bỏ qua
                cout << "Logic điều chỉnh: Tăng ngưỡng lên " << threshold << " gói/phút. Xác nhận? [Có/Không]" << endl;
            }
        } else if (response == "Có") {
            sensitivity = min(sensitivity + 0.1, 1.0);  // Tăng độ nhạy, không vượt 1.0
            ignoreCount[key] = 0;  // Reset đếm bỏ qua
        }
    }
    // ham dieu chinh dua tren xac suat rui ro .. -> kiem tra cheo // 
    double calculateRiskScore(const Packet& packet, int count) 
    {
        double score = 0.0;
        if (packet.protocol == "UDP") score += 0.3;
        if (count > 40) score += 0.5;
        if (packet.payload_size > 1000) score += 0.2;
        return score; // Điểm rủi ro từ 0.0 đến 1.0
    }
// Giám sát hành vi người dùng HITL
    void monitorUserBehavior(const string& user_id, const string& response) 
    {
        if (response == "No") 
        {
            userRejectionCount[user_id]++;
            if (userRejectionCount[user_id] >= REJECTION_THRESHOLD)
            {
                cout << "Warning: Users " << user_id << " Denied too much times.Send the notify to admin." << endl;
                userRejectionCount[user_id] = 0;  // Reset sau khi cảnh báo
            }
        }
    }

    // Kiểm tra chéo giữa phản hồi HITL và rủi ro
    void crossValidate(const Packet& packet, int count, const string& response, const string& user_id) 
    {
        double riskScore = calculateRiskScore(packet, count);
        if (riskScore > 0.7 && response == "No") 
        {
            cout << "Searching : high risk (" << riskScore << ") But users can " << user_id << " denied. Send the warning to admin." << endl;
        }
    }

    // Chuyển sang chế độ dự phòng
    void enterFallbackMode(const string& user_id) 
    {
        fallbackMode = true;
        threshold = DEFAULT_THRESHOLD;  // Chuyển sang ngưỡng an toàn
        cout << "Turns into automatic mode: Found violent from " << user_id << ". notification from admin." << endl;
    }

    // Ghi nhật ký minh bạch và gửi đến server
    void logHITLAction(const string& user_id, const string& action, const string& response) 
    {
        string logEntry = "User: " + user_id + ", Action: " + action + ", Response: " + response + ", Time: " + getCurrentTime();
        transparencyLog.push_back(logEntry);
        sendLogToServer(logEntry);  // Gửi đến server (giả lập)
    }

    // Gửi log đến server (giả lập)
    void sendLogToServer(const string& logEntry) 
    {
        // Trong thực tế, gửi qua API hoặc cơ sở dữ liệu server
        cout << "Send to server: " << logEntry << endl;
    }

    // Điều chỉnh logic dựa trên phản hồi HITL
    void adjustLogic(const string& key, const string& response, const string& user_id) {
        logHITLAction(user_id, proposeAction(key, packetCount[key]), response);
        if (response == "No") 
        {
            ignoreCount[key]++;
            if (ignoreCount[key] >= MAX_IGNORE) 
            {
                threshold += 10;  // Tăng ngưỡng nếu bỏ qua quá nhiều
                ignoreCount[key] = 0;  // Reset đếm bỏ qua
                cout << "Dynamic Logic : up the count " << threshold << " packet/60s. Accept? [Yes/No]" << endl;
            }
        } 
        else if (response == "Yes") 
        {
            sensitivity = min(sensitivity + 0.1, 1.0);  // Tăng độ nhạy, không vượt 1.0
            ignoreCount[key] = 0;  // Reset đếm bỏ qua
        }
    }

public:
    // Hàm chính xử lý raw data
    void processRawData(Packet packet) {
        // Bước 1: Cập nhật số gói tin
        updatePacketCount(packet);

        // Bước 2: Lấy số gói tin từ src_ip trong phút hiện tại
        string key = packet.src_ip + "_" + packet.timestamp.substr(0, 16);
        int count = packetCount[key];

        // Bước 3: Đề xuất hành động
        string action = proposeAction(key, count);
        if (action != "Không có hành động.") {
            cout << action << endl;

            // Bước 4: Nhận phản hồi từ HITL
            string response;
            cin >> response;

            // Bước 5: Điều chỉnh logic dựa trên phản hồi
            adjustLogic(key, response);
        }

        // Bước 6: Lưu dữ liệu để học hỏi (giả lập)
        // (Có thể lưu vào database trong thực tế)
        cout << "Dữ liệu đã xử lý: src_ip " << packet.src_ip << " - " << count << " gói/phút" << endl;
    }

    // Khởi tạo logic động
    LogicDynamic() {
        sensitivity = 0.5;  // Độ nhạy mặc định
        threshold = 30;     // Ngưỡng mặc định
    }
};

// Hàm main để chạy thử
int main() {
    LogicDynamic safeAI;

    // Ví dụ raw data
    Packet p1 = {"192.168.1.10", "203.0.113.5", "UDP", "2025-05-03 01:00:00", 512};
    Packet p2 = {"192.168.1.10", "203.0.113.5", "UDP", "2025-05-03 01:00:10", 512};
    // ... (thêm nhiều gói tin để đạt 50 gói/phút)

    // Xử lý từng gói tin với user_id giả định
    string user_id = "user_1";
    for (const Packet& packet : rawData) 
    {
        safeAI.processRawData(packet, user_id);
    }

    return 0;
}
