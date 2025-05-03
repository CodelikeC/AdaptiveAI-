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
};

// Lớp LogicDynamic đại diện cho logic động
class LogicDynamic 
{
private:
    map<string, int> packetCount;      // Đếm số gói tin từ mỗi src_ip trong 1 phút
    map<string, int> ignoreCount;      // Đếm số lần người dùng bỏ qua cảnh báo cho src_ip
    double sensitivity;                // Độ nhạy (mặc định: 0.5, từ 0 đến 1)
    int threshold;                     // Ngưỡng cảnh báo (mặc định: 30 gói/phút)
    const int MAX_IGNORE = 5;          // Ngưỡng tối đa bỏ qua để điều chỉnh

    // Hàm cập nhật số gói tin từ raw data
    void updatePacketCount(Packet packet) 
    {
        string key = packet.src_ip + "_" + packet.timestamp.substr(0, 16); // Ghép src_ip và phút
        packetCount[key]++;  // Tăng số gói tin trong phút hiện tại
    }

    // Hàm đề xuất hành động dựa trên ngưỡng
    string proposeAction(string key, int count) {
        double adjustedThreshold = threshold * sensitivity;
        if (count > adjustedThreshold) {
            return "Đề xuất: Tăng độ nhạy lên " + to_string(sensitivity + 0.1) +
                   " và gửi cảnh báo mã độc. Xác nhận? [Có/Không]";
        }
        return "Không có hành động.";
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

    // Xử lý dữ liệu
    safeAI.processRawData(p1);
    safeAI.processRawData(p2);
    // ... (lặp lại cho đủ 50 gói)

    return 0;
}