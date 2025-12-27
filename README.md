# 🏭 Industrial IoT Data Acquisition & Predictive Maintenance System

![Status](https://img.shields.io/badge/Status-Completed-success)
![Platform](https://img.shields.io/badge/Platform-STM32%20%7C%20IoT-blue)
![Domain](https://img.shields.io/badge/Domain-Predictive%20Maintenance%20%7C%20Data%20Engineering-orange)

## 📋 Project Abstract
This project implements a robust **Industrial IoT (IIoT) Data Pipeline** designed for the health monitoring of rotating machinery (e.g., CNC spindles, Induction Motors). By leveraging **Edge Computing** on the STM32F446RE, the system captures, pre-processes, and transmits high-frequency telemetry data (Vibration, Temperature, Humidity) to a cloud backend.

The primary objective is to facilitate **Predictive Maintenance (PdM)** strategies by generating a high-fidelity time-series dataset capable of training Machine Learning models for anomaly detection and fault classification.

---

## 📊 Data Science & Analytics Relevance
This project serves as the **Data Engineering / Data Acquisition** layer for Industrial AI applications.

### 1. Edge-Level Feature Extraction
Instead of transmitting raw noisy signals, the **STM32 Cortex-M4** acts as an Edge Node, performing signal conditioning and data normalization locally. This reduces cloud latency and bandwidth usage—a critical requirement for scalable MLOps.

### 2. Time-Series Dataset Generation
The system generates a structured multivariate time-series dataset ideal for:
*   **Anomaly Detection:** Identifying outliers in vibration patterns (e.g., Isolation Forest).
*   **Forecasting:** Predicting thermal runaway using regression models (e.g., ARIMA/LSTM).
*   **Classification:** Distinguishing between mechanical looseness vs. imbalance based on axis-specific G-force deltas.

### 3. Real-Time Telemetry Visualization
Utilizes **ThingSpeak** for live dashboarding, mimicking industrial SCADA systems for operational technology (OT) analytics.

---

## ⚙️ Technical Stack

| Layer | Technology Used | Description |
| :--- | :--- | :--- |
| **Edge Controller** | STM32 Nucleo-F446RE | 84MHz ARM Cortex-M4, Hardware FPU for decimal processing. |
| **Sensing** | MEMS ADXL345 | Digital Accelerometer ($\pm$4g range) for vibration capture. |
| **Environmental** | DHT22 | Precision temperature & humidity sensor for thermal profiling. |
| **Communication** | ESP8266 (ESP-01) | UART-to-WiFi Bridge using AT Command firmware. |
| **Cloud/Data** | ThingSpeak API | REST API for data ingestion and MATLAB-based visualization. |
| **Firmware** | Embedded C | Low-level drivers, Interrupt handling, and Memory Management. |

---

## 📂 Data Schema
The system streams a **5-dimensional vector** to the cloud every 16 seconds. This data can be exported as `.CSV` for model training.

| Feature Name | Data Type | Unit | Description |
| :--- | :--- | :--- | :--- |
| `Field 1` | Float | °C | Motor Surface Temperature |
| `Field 2` | Float | % | Ambient Humidity |
| `Field 3` | Float | g | Vibration - X Axis (Lateral) |
| `Field 4` | Float | g | Vibration - Y Axis (Axial) |
| `Field 5` | Float | g | Vibration - Z Axis (Vertical) |

---

## 🚀 Key Implementations
1.  **Critical Section Handling:** Implemented interrupt masking (`__disable_irq`) to ensure atomic operations during microsecond-level sensor reads, preventing race conditions.
2.  **Floating Point Optimization:** Configured the STM32 FPU and `newlib-nano` to efficiently handle decimal formatting for cloud transmission.
3.  **HMI Integration:** Concurrent operation of an I2C LCD for on-site operator feedback alongside the cloud telemetry stream.

---

## 📈 Data Visualization & Results
The system successfully streams telemetry data to the cloud. Below are the visualizations generated from the ThingSpeak API.

### 1. Vibration Analysis (X-Axis)
*Detects Lateral displacement and imbalance.*
![Vib X](Results/vib_x.png)

### 2. Vibration Analysis (Y-Axis)
*Detects Axial displacement and shaft misalignment.*
![Vib Y](Results/vib_y.png)

### 3. Vibration Analysis (Z-Axis)
*Detects Vertical impact and structural looseness.*
![Vib Z](Results/vib_z.png)

### 4. Thermal Profiling
*Correlates Temperature rise with Humidity levels.*
![Temp Graph](Results/temp_hum.png)

> *Note: Spikes in the Z-Axis graph correspond to simulated mechanical unbalance events introduced during testing.*


## 🔮 Future Scope (AI/ML)
*   **On-Device FFT:** Implementing Fast Fourier Transform using the ARM CMSIS-DSP library to convert time-domain vibration data into frequency-domain for spectral fault signature analysis.
*   **Predictive Models:** Training a Linear Regression model on collected data to predict "Time to Failure" (RUL - Remaining Useful Life).

---

### 👥 Project Contributors

**Shivaprasad V Toggi** (Team Lead)  
**Varun Gowda J**  
**Sneha K M**  
**Sinchana**  

