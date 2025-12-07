# Industrial Machine Health Monitoring System (Predictive Analysis)

### 🚀 Project Overview
**Team Name:** ZODIAC  
**Target:** Industrial Industry 4.0 Applications (CNC Machines, Induction Motors)  
**Tech Stack:** Embedded C, Python, IoT  

This project utilizes the **STM32F446RE** microcontroller to perform **Edge Computing**. It analyzes vibration (FFT) and thermal data in real-time to predict mechanical failures before they occur.

---

### 🛠 Hardware Architecture
| Component | Function |
|-----------|----------|
| **STM32 Nucleo-F446RE** | Main Processing Unit (Cortex-M4 + FPU) |
| **ADXL345** | 3-Axis MEMS Accelerometer (Vibration Analysis) |
| **ACS712** | Current Sensor (Load Monitoring) |
| **DHT22** | Temperature & Humidity Sensor |
| **ESP8266 (ESP-01)** | Wi-Fi Module for IoT Dashboard |
| **16x2 LCD** | On-site Status Display |

---

### 🧠 Key Features
1.  **Predictive Maintenance:** Uses Fast Fourier Transform (FFT) on the chip to detect specific fault frequencies (e.g., Bearing Faults vs. Unbalance).
2.  **IoT Dashboard:** Real-time data visualization on ThingSpeak.
3.  **Edge Processing:** Signal processing happens on the STM32, not on the cloud.

---

### 📂 Repository Structure
*   `STM32_Code/`: Source code for the Nucleo-F446RE (HAL Drivers + CMSIS-DSP).
*   `Python_Scripts/`: Scripts for generating block diagrams and literature review tables.
*   `Diagrams/`: System Architecture and Circuit Diagrams.
*   `Docs/`: Project Synopsis and Reference Papers.

---

### 👥 Team Members
*   **Shivaprasad V Toggi** (Team Lead)
*   Varun Gowda J
*   Sneha K M
*   Sinchana
