# 🏭 Industrial Machine Health Monitoring System (IoT)

### 🚀 Project Overview
**Team:** ZODIAC  
**Platform:** STM32 Nucleo-F446RE | ESP8266 | ThingSpeak  
**Status:** ✅ Completed

This project is an **Edge Computing-based Predictive Maintenance System**. It monitors industrial machinery (induction motors, CNC spindles) to detect faults like **Mechanical Unbalance** and **Thermal Runaway** before catastrophic failure occurs.

---

### 🧠 Key Features
1.  **Edge Processing:** Signal conditioning and data conversion happens locally on the STM32 (ARM Cortex-M4), reducing cloud latency.
2.  **Real-Time Visualization:** Live graphs of Vibration (X, Y, Z) and Temperature on **ThingSpeak**.
3.  **Critical Section Logic:** Implemented interrupt masking to ensure precise microsecond timing for DHT22 sensors.
4.  **Local Feedback:** On-site LCD displays system status and sensor values for operators.

---

### 🛠️ Hardware Stack
| Component | Purpose | Protocol |
|-----------|---------|----------|
| **STM32F446RE** | Main Controller | - |
| **ADXL345** | 3-Axis Vibration Sensor | I2C |
| **DHT22** | Temp & Humidity Sensor | GPIO (1-Wire) |
| **ESP8266 (ESP-01)** | Wi-Fi Gateway | UART (AT Commands) |
| **16x2 LCD** | Local Status Display | I2C |

---

### 📊 Results & Dashboard
**Real-Time Vibration Analysis:**
![ThingSpeak Graph](Results/ThingSpeak_Vibration.png)
*(Replace this text with the actual link to your image after uploading)*

**Hardware Prototype:**
![Prototype](Results/Hardware_Setup.jpg)

---

### 🔌 Wiring Connections
| Sensor Pin | STM32 Pin | Note |
|------------|-----------|------|
| **ADXL SDA** | PB9 (D14) | I2C1 |
| **ADXL SCL** | PB8 (D15) | I2C1 |
| **DHT22 Data**| PB5 (D4) | GPIO |
| **ESP8266 RX** | PA9 (D8) | UART1 |
| **ESP8266 TX** | PA10 (D2) | UART1 |

---

### ⚙️ How to Run
1.  Open the project in **STM32CubeIDE**.
2.  Update `main.c` with your Wi-Fi SSID, Password, and ThingSpeak API Key.
3.  Connect the Nucleo Board via USB.
4.  Build and Flash the code.
5.  View data on the Serial Monitor (115200 Baud) or ThingSpeak.
