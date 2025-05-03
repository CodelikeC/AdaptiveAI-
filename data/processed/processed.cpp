#include <iostream>  

#include <algorithm>
#include <string> 
#include <map>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// Cấu trúc dữ liệu cho raw data (gói tin mạng)
struct Packet {
    string src_ip;         // Địa chỉ IP nguồn
    string dest_ip;        // Địa chỉ IP đích
    string protocol;       // Giao thức (UDP, TCP, v.v.)
    string timestamp;      // Thời gian (YYYY-MM-DD HH:MM:SS)
    int payload_size;      // Kích thước gói tin (bytes)

    // Hàm khởi tạo
    Packet(string s_ip, string d_ip, string prot, string ts, int size) 
        : src_ip(s_ip), dest_ip(d_ip), protocol(prot), timestamp(ts), payload_size(size) {}
};

// Lớp LogicDynamic để xử lý logic động
class LogicDynamic {
private:
    map<string, int> packetCount;      // Đếm số gói tin từ mỗi src_ip trong 1 phút
    map<string, int> ignoreCount;      // Đếm số lần bỏ qua cảnh báo cho mỗi src_ip
    double sensitivity;                // Độ nhạy (0.0 đến 1.0)
    int threshold;                     // Ngưỡng cảnh báo (gói/phút)
    const int MAX_IGNORE = 5;          // Ngưỡng tối đa bỏ qua để điều chỉnh

    // Cập nhật số gói tin từ raw data
    void updatePacketCount(const Packet& packet) {
        string key = packet.src_ip + "_" + packet.timestamp.substr(0, 16); // Lấy phút (YYYY-MM-DD HH:MM)
        packetCount[key]++;
    }

    // Đề xuất hành động dựa trên ngưỡng
    string proposeAction(const string& key, int count) {
        double adjustedThreshold = threshold * sensitivity;
        if (count > adjustedThreshold) {
            return "Đề xuất: Tăng độ nhạy lên " + to_string(sensitivity + 0.1) +
                   " và gửi cảnh báo mã độc. Xác nhận? [Có/Không]";
        }
        return "Không có hành động.";
    }

    // Điều chỉnh logic dựa trên phản hồi HITL
    void adjustLogic(const string& key, const string& response) {
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

public:
    // Xử lý raw data
    void processRawData(const Packet& packet) {
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

        // Bước 6: Hiển thị kết quả đã xử lý
        cout << "Dữ liệu đã xử lý: src_ip " << packet.src_ip << " - " << count << " gói/phút" << endl;
    }

    // Khởi tạo logic động
    LogicDynamic() : sensitivity(0.5), threshold(30) {}
};

// Hàm main để chạy chương trình
int main() {
    LogicDynamic safeAI;

    // Ví dụ raw data (có thể đọc từ file hoặc sensor trong thực tế)
    vector<Packet> rawData = {
        Packet("192.168.1.10", "203.0.113.5", "UDP", "2025-05-03 01:00:00", 512),
        Packet("192.168.1.10", "203.0.113.5", "UDP", "2025-05-03 01:00:10", 512),
        // Thêm nhiều gói tin để đạt 50 gói/phút (có thể lặp trong vòng lặp thực tế)
    };

    // Xử lý từng gói tin
    for (const Packet& packet : rawData) {
        safeAI.processRawData(packet);
    }

    return 0;
}