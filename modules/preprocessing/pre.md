# 📂 `preprocessing/` — Adaptive AI

### *Vietnamese – English README*

---

## 🇻🇳 Giới thiệu

Folder **`preprocessing/`** là tầng đầu tiên trong pipeline của **Adaptive AI**.
Nhiệm vụ chính: **làm sạch – chuẩn hóa – trích chọn – tái cấu trúc dữ liệu** trước khi được đưa vào các module phân tích, mô hình trạng thái, hành vi, bảo vệ, và học thích nghi.

Adaptive AI hoạt động trong môi trường phân tán, thời gian thực và có đặc tính bất định.
Vì vậy, preprocessing không chỉ là bước tiền xử lý đơn thuần mà còn là **lớp bảo vệ đầu**, giúp:

* Loại bỏ tín hiệu bẩn / bất thường
* Phát hiện dữ liệu khả nghi
* Chuẩn hóa định dạng theo giao thức của AICP
* Giảm nhiễu cho các thuật toán Behaviour Engine và Meta-Learning
* Tăng ổn định cho các vòng phản hồi (feedback loops)

---

## 🇬🇧 Overview

The **`preprocessing/`** folder is the initial layer of the Adaptive AI pipeline.
Its role is to **clean, normalize, extract, and restructure data** before it reaches the analytical, behavioral, protection, and adaptation components.

In a distributed, real-time, and uncertain environment, preprocessing acts not only as a data preparation stage but also as a **first-line defensive layer**, enabling:

* Noise and anomaly filtering
* Suspicious input detection
* Protocol-level normalization (AICP unified protocol)
* Reduced noise for the Behavior Engine and Meta-Learning modules
* Increased stability in recurrent feedback loops

---

## 📁 Cấu trúc thư mục — Folder Structure

**Hiện tại** (current modules):

```
preprocessing/
│
├── data_cleaner.h / data_cleaner.cpp
├── feature_extraction.h / feature_extraction.cpp
│
└── (đang mở rộng thêm các module mới)
```

**Dự kiến mở rộng** (planned extensions):

```
preprocessing/
│
├── data_cleaner/            # Lọc, chuẩn hóa, loại nhiễu
├── feature_extraction/      # Trích đặc trưng đa tầng
├── anomaly_pre_filter/      # Phát hiện bất thường cấp 0 (pre-anomaly)
├── schema_normalizer/       # Chuẩn hóa JSON / binary theo schema
├── temporal_smoothing/      # Làm mượt tín hiệu theo thời gian
├── semantic_mapper/         # Bản đồ hóa ngữ nghĩa, dùng trước meta-learning
└── adaptive_preprocessor/   # Preprocessing thích nghi theo trạng thái hệ thống
```

---

## 🧩 Vai trò kỹ thuật — Technical Responsibilities

### 🇻🇳 Tiếng Việt

`preprocessing/` phải đảm bảo:

1. **Tính nhất quán dữ liệu:** mọi đầu vào phải được áp chuẩn trước khi đi vào lõi Adaptive AI.
2. **Giảm tải cho các module nặng:** Behavior Engine, State Model, Protection.
3. **Tích hợp tốt với Lua/Rust/C++ plugin:** cho phép linh hoạt mở rộng.
4. **Chống tấn công đầu vào:** bao gồm injection, malformed packets, schema violation.
5. **Tự thích nghi:** có khả năng điều chỉnh chiến lược làm sạch dựa trên trạng thái hệ thống.

### 🇬🇧 English Version

`preprocessing/` must guarantee:

1. **Data consistency:** all inputs are normalized before entering the Adaptive AI core.
2. **Load reduction:** protects heavy modules such as Behavior Engine or State Model from dirty signals.
3. **Cross-language extensibility:** integrates smoothly with Lua/Rust/C++ plugins.
4. **Input-layer defense:** guards against injection, malformed packets, and schema violations.
5. **Adaptive behavior:** preprocessing strategies adjust according to system conditions.

---

## 🔧 Các module chính — Main Modules

### **1. `data_cleaner`**

**VN:** Xử lý nhiễu, loại bỏ trường thừa, khôi phục trường thiếu, chuẩn hóa format.
**EN:** Noise removal, field normalization, missing-field recovery.

---

### **2. `feature_extraction`**

**VN:** Trích xuất đặc trưng đa tầng: thống kê, ngữ nghĩa, cấu trúc, thời gian.
**EN:** Multi-layer feature extraction: statistical, semantic, structural, temporal.

---

### **3. Các module mở rộng (optional but recommended)**

#### **`anomaly_pre_filter`**

VN: Phát hiện anomaly sơ bộ trước khi truyền vào detector chính.
EN: Pre-anomaly detection before main detectors.

#### **`schema_normalizer`**

VN: Áp chuẩn theo `message_schema.json`, tránh dữ liệu sai lệch.
EN: Ensures strict compliance with unified schemas.

#### **`adaptive_preprocessor`**

VN: Thay đổi thuật toán preprocessing theo mô hình trạng thái (state model).
EN: Dynamically adjusts preprocessing strategies based on system state.

---

## 🔌 Tích hợp — Integration

Preprocessing kết nối trực tiếp với:

```
sensors/ → preprocessing/ → modules/analysis → behavior engine → runtime/
```

Nó cũng có hooks để:

* Lua scripts
* Rust plugins
* C/C++ behavior transformers
* Protection pipeline (detector → self-defense → rollback engine)

---

## 🚀 Tầm quan trọng chiến lược — Strategic Importance

### 🇻🇳

Preprocessing quyết định **70% độ ổn định** của Adaptive AI vì toàn bộ hệ thống phía sau sẽ học và phản ứng dựa trên dữ liệu được chuẩn hóa ở tầng này.

### 🇬🇧

Preprocessing determines **70% of system stability**, as all downstream learning and behavioral responses rely on this stage’s output.

---

## 📜 Giấy phép — License

Folder này tuân theo **Apache 2.0** giống toàn bộ Adaptive AI.

---

## 🧠 Gợi ý phát triển tiếp theo — Next Expansion Ideas

* Thêm **fuzzy-cleaning engine** (fuzzy logic-based noise tolerance)
* Thêm **Bayesian pre-filter**
* Thêm **temporal consistency checker**
* Thêm **trust-score annotator** để hỗ trợ Protection Engine

---

