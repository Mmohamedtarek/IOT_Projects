# 🌍 Air Quality Monitor using ESP32

A smart **IoT-based Air Quality Monitoring System** built with **ESP32** that continuously monitors the surrounding environment using a **gas sensor** and a **DHT22 temperature & humidity sensor**.

The system displays real-time environmental data on an **SSD1306 OLED display**, triggers a **visual and audible alarm** when poor air quality is detected, and outputs sensor readings to the Serial Monitor for debugging and monitoring.

---

## 📌 Features

- 🌡️ Real-time temperature monitoring
- 💧 Real-time humidity monitoring
- 🧪 Air quality monitoring using an analog gas sensor
- 📺 Live data displayed on a 128×64 OLED screen
- 🔴 LED warning indicator for poor air quality
- 🔊 Buzzer alarm when gas concentration exceeds the danger threshold
- 🖥️ Serial Monitor output for debugging and logging
- ⚡ Built using the Arduino framework on ESP32
- 🧪 Fully compatible with Wokwi simulation

---

## 🛠 Hardware Components

| Component | Quantity |
|-----------|---------:|
| ESP32 DevKit V4 | 1 |
| DHT22 Temperature & Humidity Sensor | 1 |
| Analog Gas Sensor (MQ-style) | 1 |
| SSD1306 OLED Display (I2C) | 1 |
| Active Buzzer | 1 |
| Red LED | 1 |
| 220Ω Resistor | 1 |

---

## 🔌 Pin Connections

| ESP32 Pin | Connected Device |
|-----------|------------------|
| GPIO 4 | DHT22 Data |
| GPIO 34 | Gas Sensor Analog Output |
| GPIO 21 | OLED SDA |
| GPIO 22 | OLED SCL |
| GPIO 26 | Buzzer |
| GPIO 27 | LED |
| 3.3V | DHT22 + OLED |
| 5V | Gas Sensor |
| GND | Common Ground |

---

## 📊 Air Quality Levels

The air quality is determined based on the analog gas sensor reading.

| Gas Value | Status |
|-----------:|--------|
| < 1000 | ✅ GOOD |
| 1000 – 2499 | ⚠️ MODERATE |
| ≥ 2500 | ❌ BAD |

When the air quality becomes **BAD**:

- The red LED turns ON.
- The buzzer emits an alarm.
- The OLED displays the warning status.

---

## 🖥 OLED Display

The OLED shows:

- Air Quality Status
- Gas Sensor Reading
- Temperature (°C)
- Humidity (%)

Example:

```
Air: GOOD
Gas: 845
Temp: 26.3 C
Hum: 48.2 %
```

---

## 🖨 Serial Monitor Output

Example:

```
Gas: 845 | Air: GOOD | Temp: 26.30 C | Humidity: 48.20%
```

---

## 📁 Project Structure

```
Air-Quality-Monitor/
│
├── src/
│   └── main.cpp
│
├── diagram.json
│
├── platformio.ini
│
└── README.md
```

---

## 📚 Libraries

The project uses the following Arduino libraries:

- Adafruit GFX Library
- Adafruit SSD1306
- DHT Sensor Library

PlatformIO automatically installs them from:

```ini
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SSD1306
    adafruit/DHT sensor library
```

---

## ▶️ Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/air-quality-monitor.git
```

### 2. Open with PlatformIO

Open the project using **Visual Studio Code** with the **PlatformIO** extension installed.

### 3. Build

```bash
pio run
```

### 4. Upload

```bash
pio run --target upload
```

### 5. Monitor Serial Output

```bash
pio device monitor
```

---

## 🧪 Wokwi Simulation

The project includes a complete **diagram.json** file, making it ready to run directly in **Wokwi** without additional configuration.

---

## 🚀 Possible Future Improvements

- MQTT integration
- Wi-Fi connectivity
- ThingSpeak dashboard
- Blynk mobile application
- Cloud data logging
- Email/SMS notifications
- Telegram alerts
- Web dashboard
- AQI (Air Quality Index) calculation
- Multiple gas sensor support

---

## 🛠 Technologies Used

- ESP32
- Arduino Framework
- PlatformIO
- C++
- Wokwi Simulator
- I2C Communication

---

## 📄 License

This project is intended for educational and learning purposes. Feel free to modify and extend it for your own IoT applications.

---

## 👨‍💻 Author

**Mohamed Tarek**

Engineering Student 