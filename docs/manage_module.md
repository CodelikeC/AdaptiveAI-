### 1. **Tái Cấu Trúc Modules**
Đã có các module như `ThreatAnalyzer`, `KillSwitchMonitor`, v.v. giữ nguyên logic nhưng tổ chức lại:

- **`protection/threat_analyzer.ixx` (Interface):**
  ```cpp
  export module Protection:ThreatAnalyzer;

  export struct ThreatSignal {
      float severityScore;
  };

  export enum class ThreatLevel {
      SAFE, LOW, MEDIUM, HIGH, CRITICAL
  };

  export class ThreatAnalyzer {
  public:
      ThreatAnalyzer();
      ~ThreatAnalyzer();
      void ingestSignal(const ThreatSignal& signal);
      std::vector<ThreatSignal> getRecentSignals();
      void clearSignals();
      float computeAggregateSeverity();
      ThreatLevel classifyThreatLevel(float score);
      ThreatLevel analyzeThreat();
  private:
      std::vector<ThreatSignal> signalBuffer;
      std::mutex bufferMutex;
  };
  ```

- **`protection/threat_analyzer.cpp` (Implementation):**
  ```cpp
  module Protection:ThreatAnalyzer;

  import <mutex>;
  import <vector>;

  ThreatAnalyzer::ThreatAnalyzer() {}
  ThreatAnalyzer::~ThreatAnalyzer() {}

  void ThreatAnalyzer::ingestSignal(const ThreatSignal& signal) {
      std::lock_guard<std::mutex> lock(bufferMutex);
      signalBuffer.push_back(signal);
  }

  std::vector<ThreatSignal> ThreatAnalyzer::getRecentSignals() {
      std::lock_guard<std::mutex> lock(bufferMutex);
      return signalBuffer;
  }

  void ThreatAnalyzer::clearSignals() {
      std::lock_guard<std::mutex> lock(bufferMutex);
      signalBuffer.clear();
  }

  float ThreatAnalyzer::computeAggregateSeverity() {
      std::lock_guard<std::mutex> lock(bufferMutex);
      float total = 0.0f;
      for (const auto& sig : signalBuffer) {
          total += sig.severityScore;
      }
      return signalBuffer.empty() ? 0.0f : total / signalBuffer.size();
  }

  ThreatLevel ThreatAnalyzer::classifyThreatLevel(float score) {
      if (score < 0.1f) return ThreatLevel::SAFE;
      if (score < 0.3f) return ThreatLevel::LOW;
      if (score < 0.6f) return ThreatLevel::MEDIUM;
      if (score < 0.85f) return ThreatLevel::HIGH;
      return ThreatLevel::CRITICAL;
  }

  ThreatLevel ThreatAnalyzer::analyzeThreat() {
      float score = computeAggregateSeverity();
      ThreatLevel level = classifyThreatLevel(score);

      std::string levelStr;
      switch (level) {
          case ThreatLevel::SAFE: levelStr = "SAFE"; break;
          case ThreatLevel::LOW: levelStr = "LOW"; break;
          case ThreatLevel::MEDIUM: levelStr = "MEDIUM"; break;
          case ThreatLevel::HIGH: levelStr = "HIGH"; break;
          case ThreatLevel::CRITICAL: levelStr = "CRITICAL"; break;
      }

      std::cout << "[ThreatAnalyzer] Aggregate Score: " << score
                << " | Threat Level: " << levelStr << std::endl;

      return level;
  }
  ```
- Tương tự, tạo các module khác (`KillSwitchMonitor`, `SelfDefense`, `TrustGuard`) với interface và implementation riêng.

### 2. **File Test Thủ Công**
Thay vì dùng framework, ta có thể viết một file test `test_protection.cpp` để gọi và kiểm tra các hàm thủ công, dựa trên logic đầu tiên. Đây là cách tổ chức:

```cpp
import <iostream>;
import Protection:ThreatAnalyzer;
import Protection:KillSwitchMonitor;
import Protection:SelfDefense;
import Protection:TrustGuard;

using namespace std;

int main() {
    cout << "==== Test Protection ====" << endl;
    cout << "=========================" << endl;
    cout << "=========================" << endl;

    // === Activate Self-Defense ===
    {
        SelfDefense defense;
        // Giả định có hàm activate()
        defense.activate();  // Thay bằng hàm thực tế của SelfDefense
        cout << "Self-Defense activated." << endl;
    }

    // === Activate ThreatAnalyzer ===
    {
        ThreatAnalyzer analyzer;
        ThreatSignal signal1{0.5f};
        ThreatSignal signal2{0.8f};

        analyzer.ingestSignal(signal1);
        analyzer.ingestSignal(signal2);

        float severity = analyzer.computeAggregateSeverity();
        cout << "Computed Severity: " << severity << endl;  // Kiểm tra thủ công

        ThreatLevel level = analyzer.analyzeThreat();
        // Kiểm tra logic bằng cách in ra
        if (severity == 0.65f && level == ThreatLevel::HIGH) {
            cout << "ThreatAnalyzer test passed." << endl;
        } else {
            cout << "ThreatAnalyzer test failed." << endl;
        }
    }

    // === Activate TrustGuard ===
    {
        TrustGuard guard;
        // Giả định có hàm checkTrust()
        guard.checkTrust();  // Thay bằng hàm thực tế
        cout << "TrustGuard checked." << endl;
    }

    // === Activate KillSwitchMonitor ===
    {
        KillSwitchMonitor monitor;
        // Giả định có hàm monitorStatus()
        monitor.monitorStatus();  // Thay bằng hàm thực tế
        cout << "KillSwitchMonitor running." << endl;
    }

    return 0;
}
```

### 3. **Build và Chạy**
- Cập nhật `CMakeLists.txt`:
  ```cmake
  cmake_minimum_required(VERSION 3.28)
  project(MyProject LANGUAGES CXX)

  set(CMAKE_CXX_STANDARD 23)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dcf690aaecc")
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

  add_library(Protection MODULE
      src/modules/protection/threat_analyzer.ixx
      src/modules/protection/threat_analyzer.cpp
      src/modules/protection/killswitch_monitor.ixx
      src/modules/protection/killswitch_monitor.cpp
      src/modules/protection/self_defense.ixx
      src/modules/protection/self_defense.cpp
      src/modules/protection/trust_guard.ixx
      src/modules/protection/trust_guard.cpp
  )

  add_executable(test_protection tests/test_protection.cpp)
  target_link_libraries(test_protection PRIVATE Protection)
  ```
- Build: `cmake -S . -B build && cmake --build build`.
- Chạy: `./build/test_protection`.

### 4. **Tại Sao Cách Này Bền Vững Theo First Principles**
- **Tự chủ**: Không phụ thuộc vào framework, kiểm soát hoàn toàn logic test, từ in ra kết quả đến so sánh thủ công.
- **Modularity**: Modules giữ code tách biệt, dễ mở rộng khi thêm module mới.
- **Hiệu quả**: Chỉ import những gì cần, giảm overhead so với include toàn bộ.
- **Kiểm soát vi mô**: Có thể thêm các kiểm tra thủ công (như if-else) để đảm bảo mỗi hàm hoạt động đúng, thay vì dựa vào assertion của framework.

### 5. **Mở Rộng và Tùy Chỉnh**
- Nếu muốn test sâu hơn (ví dụ: lặp qua nhiều signal), thêm vòng lặp trong `main()`.
- Nếu các module khác (KillSwitchMonitor, v.v.) chưa có code, hãy định nghĩa ít nhất một hàm public để gọi trong test.
- Để tăng độ tin cậy, có thể thêm log file hoặc biến đếm lỗi thủ công.
