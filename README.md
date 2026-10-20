# IoT-Based Water Quality Monitoring System

A student prototype using an ESP32 and sensors to monitor selected water-quality indicators and send readings to a Blynk dashboard.

## Hardware
- ESP32 Dev Module
- TDS sensor
- Turbidity sensor
- pH sensor module
- DS18B20 temperature sensor

## Pin connections used in this code
| Sensor | Sensor pin | ESP32 pin |
|---|---|---|
| TDS | Analog output (A / SP on your module) | GPIO 36 |
| Turbidity | OUT / AO output | GPIO 34 |
| pH module | O/P+ | GPIO 35 |
| pH module | O/P- / GND | GND |
| pH module | +VCC | VIN / 5V only if required by your exact module |
| pH module | -VCC | GND |
| DS18B20 | Red | 3.3V |
| DS18B20 | Black | GND |
| DS18B20 | Yellow/data | GPIO 4 |

**Important:** Confirm each module's pin labels and voltage requirements before powering it. ESP32 GPIO inputs are not 5 V tolerant. Any analog output sent to an ESP32 ADC pin must remain within the ESP32's safe input range (generally 0–3.3 V). Use a suitable voltage divider/level conversion if needed. A DS18B20 normally requires a 4.7 kΩ pull-up resistor between DATA and 3.3 V.

## Software
- Arduino IDE
- ESP32 board support installed
- Blynk library
- OneWire library
- DallasTemperature library

## Setup
1. Copy `src/secrets.example.h` to `src/secrets.h` if you choose to keep credentials in a separate header. This starter sketch currently uses placeholders directly in the `.ino`; replace them locally and **never commit real credentials**.
2. In `src/water_quality_monitor.ino`, replace the Blynk template ID, Blynk auth token, Wi-Fi name, and Wi-Fi password with your own values.
3. In Blynk, create datastreams: V0 (Double, TDS estimate), V1 (Integer/Double, turbidity raw ADC), V2 (Double, pH estimate), V3 (Double, temperature °C), V4 (String, status).
4. To use alerts, create a Blynk event with event code `water_alert` and enable notifications.
5. Install the listed Arduino libraries.
6. Open the `.ino` file in Arduino IDE, select **ESP32 Dev Module** and the correct COM port, then upload.
7. Open Serial Monitor at **115200 baud**.
8. Create widgets on the Blynk web/mobile dashboard linked to the datastreams above.

## Reading and safety notes
- The TDS and pH values in this starter code are estimates and require calibration. A generic formula is not a substitute for calibration with appropriate reference solutions.
- Turbidity is shown as raw ADC reading, not a validated turbidity percentage or NTU value. Calibrate it against a suitable reference before reporting NTU.
- A reading of `-127 °C` from a DS18B20 means the sensor is disconnected/not detected; check wiring and the data-line pull-up resistor.
- This prototype does **not** certify that water is safe to drink. Drinking-water safety requires appropriate testing for relevant chemical and microbiological contaminants.

## Project structure
```text
Water-Quality-Monitoring-System/
├── src/
│   ├── water_quality_monitor.ino
│   └── secrets.example.h
├── docs/
│   └── (add your actual circuit/dashboard screenshots here)
├── README.md
└── .gitignore
```

## Author
Pradyumn Shriwas
