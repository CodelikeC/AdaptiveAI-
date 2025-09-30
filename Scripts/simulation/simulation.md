# Tác dụng: 
→ Giúp kiểm chứng logic Adaptive AI trước khi deploy.

``` plaintext
├── simulation/
│   ├── sandbox_runner.h/.cpp        # Chạy thử logic trong môi trường cô lập
│   ├── attack_scenario.h/.cpp       # Mô phỏng các attack pattern
│   ├── rollback_simulator.h/.cpp    # Thử cơ chế rollback khi bị lỗi
│   └── adaptive_benchmark.h/.cpp    # Benchmark hiệu suất khi thay đổi policy
``` 
