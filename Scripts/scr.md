# Demo Scripts

#### Vietnamese: 
- Phần này lo về vấn đề viết logic cho khả năng thích ứng của bộ cảm biến.
- Chạy thử các tính năng của phần lõi "CORE".
- Kiểm tra khả năng trơn tru khi giao tiếp với dữ liệu thực từ con người. 

+ Nội dung triển khai

Logic thích ứng cho bộ cảm biến

Viết kịch bản thử nghiệm các tình huống khác nhau (dữ liệu bị nhiễu, mất gói tin, dữ liệu bất thường).

Mục đích: kiểm tra xem sensor module có tự điều chỉnh ngưỡng và bộ lọc theo thời gian hay không.

Ví dụ:

Sensor nhận tín hiệu nhịp tim bất thường → hệ thống thích ứng bộ lọc để loại bỏ nhiễu.

Sensor môi trường báo liên tục giá trị cao → AI điều chỉnh threshold để phân biệt giữa nguy hiểm thật và false alarm.

Chạy thử các tính năng của phần lõi “CORE”

Kiểm tra pipeline: input → preprocessing → adaptive core → output/response.

Viết demo cho từng module con:

State Model: thay đổi trạng thái để xem khả năng chuyển đổi mượt mà.

Behavior Engine: gửi dữ liệu vào, quan sát phản ứng thích ứng theo rule/learning.

Context Manager: nhập dữ liệu mới, kiểm tra khả năng lưu/truy xuất ngữ cảnh.

Giao tiếp với dữ liệu thực từ con người

Cho phép input từ bàn phím hoặc file JSON mô phỏng dữ liệu cảm xúc, hành vi, tín hiệu.

Hệ thống cần phản hồi tức thì, tránh lag hoặc deadlock.

Ví dụ demo:

Người dùng nhập: “Tôi cảm thấy lo lắng.” → CORE phân tích và chọn phản ứng phù hợp (giảm nhịp độ, tăng ngưỡng phòng thủ).

Người dùng nhập dữ liệu hành động liên tục (như chuỗi lệnh JSON) → hệ thống vẫn giữ ổn định, không crash.

Kịch bản Regression Test & Stress Test

Stress test: bơm dữ liệu lớn và liên tục, theo dõi hiệu năng.

Regression test: đảm bảo khi cập nhật logic mới, các phản ứng cũ vẫn hoạt động bình thường.

Kết quả mong đợi

Adaptive AI hoạt động ổn định trong môi trường demo.

Sensor tự hiệu chỉnh được, CORE phản ứng hợp lý.

Giao tiếp dữ liệu người dùng → phản hồi tự nhiên, mượt.

#### English: 

- This section is about writing logic for the adaptive capabilities of the sensor.

- Test the features of the "CORE" core.

- Test the smoothness when communicating with real data from humans.

+ Implementation content

Adaptive logic for the sensor

Write test scripts for different scenarios (noisy data, lost packets, abnormal data).

Purpose: check whether the sensor module can automatically adjust the threshold and filter over time.

For example:

Sensor receives an abnormal heartbeat signal → the system adapts the filter to remove the noise.

Environmental sensor continuously reports high values ​​→ AI adjusts the threshold to distinguish between real danger and false alarm.

Test the features of the "CORE" core

Test pipeline: input → preprocessing → adaptive core → output/response.

Write a demo for each sub-module:

State Model: change the state to see the smooth transition.

Behavior Engine: send data in, observe the adaptive response according to the rule/learning.

Context Manager: import new data, test the ability to save/retrieve context.

Interact with real data from humans

Allow input from keyboard or JSON file to simulate emotional data, behavior, signal.

The system needs to respond immediately, avoid lag or deadlock.

Example demo:

User enters: "I feel anxious." → CORE analyzes and chooses the appropriate response (slow down the pace, increase the defense threshold).

User enters continuous action data (like JSON command string) → the system remains stable, does not crash.

Regression Test & Stress Test Scenario

Stress test: pump large and continuous data, monitor performance.

Regression test: ensure that when updating new logic, old reactions still work normally.

Expected results

Adaptive AI works stably in the demo environment.

Sensor self-calibrates, CORE reacts reasonably.

User data communication → natural, smooth response.