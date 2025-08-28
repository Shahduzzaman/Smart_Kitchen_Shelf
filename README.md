# Smart Shelf — IoT-Based Kitchen Ingredient Monitoring

> Real-time ingredient level tracking with **ESP32 + HX711 + Load Cells**, live charts & low-stock alerts via **Blynk**.

---

## What it does

- Measures container weight continuously using **5 kg / 1 kg load cells** + **HX711** amplifiers  
- Pushes readings from **ESP32** to **Blynk IoT** over Wi-Fi  
- Shows ingredient levels on phone/dashboard
- Designed for **home kitchens** 

---

## Hardware Overview

| Module | Qty | Notes |
|---|---:|---|
| ESP32 DevKit (NodeMCU-32S) | 1 | Wi-Fi MCU |
| HX711 24-bit ADC | 3 | One per load cell |
| Load cell 5 kg | 2 | For rice, sugar, etc. |
| Load cell 1 kg | 1 | For spices/salt |
| Frame/Stand | 3 | Holds containers & sensors |
| Power (5 V USB) | 1 | Phone charger recommended |
| Optional: Battery + Buck | 1 | Backup power |

---

## System Diagram

<p align="center">
  <img width="864" height="435" alt="System Diagram" src="https://github.com/user-attachments/assets/8ca78a80-285c-4682-8961-4c29bd0d1838" />
</p>

---

## ⚙️ Load Cell Mounting (Recommended)

<p align="center">
  <img width="919" height="481" alt="Load cell Setup" src="https://github.com/user-attachments/assets/8565b205-149c-4a87-90a4-fdf400484180" />
</p>

**Tips**  
- Fix **one end** of the bar to the frame and let the **other end** support the container platform.  
- Do not clamp across the sensing cavity. Use spacers to avoid side-loads & torque.  
- Use identical geometry for cells that you want to compare directly.

---

## 🔌 Wiring / Circuit

<p align="center">
  <img alt="Circuit Diagram" src="https://github.com/user-attachments/assets/c6d2e114-94de-4f4b-bf36-2600131756ee" />
</p>

### Pin Map (example)

| HX711 | ESP32 | Notes |
|---|---|---|
| VCC | 3V3 or 5V | Keep stable |
| GND | GND | Common ground |
| DT | GPIO 23 | Data |
| SCK | GPIO 22 | Clock |
| E+ | Load Cell Red | Excitation + |
| E− | Load Cell Black | Excitation − |
| A+ | Load Cell White | Signal + |
| A− | Load Cell Green | Signal − |

> You can assign different GPIOs per channel. Keep wires short and twisted (E+/E− together, A+/A− together) to reduce noise.

---

## Setup (MVP)

1. **Firmware** (Arduino IDE)  
   - Install boards: *ESP32 by Espressif*  
   - Libraries: `HX711`, `Blynk`  
   - Configure Wi-Fi & Blynk credentials, set GPIO pins for each HX711.

2. **Blynk Console/App**  
   - Create template, add datastreams (e.g., V0/V1/V2 for each ingredient).  
   - Add gauge/label widgets and **automation**: if weight \< threshold → push notification.

3. **Calibration** (per load cell)

