#define BLYNK_TEMPLATE_ID   "PASTE_YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Water Quality Monitor"
#define BLYNK_AUTH_TOKEN    "PASTE_YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Wi-Fi credentials: fill these in before uploading.
// Do not upload real passwords/tokens to a public GitHub repository.
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// Pins based on your current wiring
#define TDS_PIN       36
#define TURBIDITY_PIN 34
#define PH_PIN        35
#define TEMP_PIN      4

OneWire oneWire(TEMP_PIN);
DallasTemperature tempSensor(&oneWire);
BlynkTimer timer;

void readSensorsAndSend() {
  // Raw analog readings from ESP32 ADC (0-4095)
  int tdsRaw = analogRead(TDS_PIN);
  int turbRaw = analogRead(TURBIDITY_PIN);
  int phRaw = analogRead(PH_PIN);

  // These are starter estimates, not calibrated measurements.
  float tdsVoltage = tdsRaw * (3.3f / 4095.0f);
  float tdsPpmEstimate = tdsVoltage * 1000.0f;

  float turbVoltage = turbRaw * (3.3f / 4095.0f);

  float phVoltage = phRaw * (3.3f / 4095.0f);
  // Approximate formula only. Calibrate with known buffer solutions.
  float phEstimate = 7.0f + ((2.5f - phVoltage) / 0.18f);

  tempSensor.requestTemperatures();
  float temperatureC = tempSensor.getTempCByIndex(0);
  bool tempValid = (temperatureC != DEVICE_DISCONNECTED_C &&
                    temperatureC > -55.0f && temperatureC < 125.0f);

  // Send readings to Blynk datastreams:
  // V0 = TDS estimate (ppm), V1 = turbidity raw ADC, V2 = pH estimate,
  // V3 = temperature (C), V4 = status text.
  Blynk.virtualWrite(V0, tdsPpmEstimate);
  Blynk.virtualWrite(V1, turbRaw);
  Blynk.virtualWrite(V2, phEstimate);

  bool potentiallyUnsafe = (phEstimate < 6.5f || phEstimate > 8.5f ||
                            turbRaw == 0 || !tempValid);

  if (tempValid) {
    Blynk.virtualWrite(V3, temperatureC);
  } else {
    Blynk.virtualWrite(V3, -127.0f); // indicates invalid/disconnected reading
  }

  if (potentiallyUnsafe) {
    Blynk.virtualWrite(V4, "CHECK WATER");
    // Create a Blynk event named "water_alert" and enable notifications
    // in the Blynk console before using this event.
    Blynk.logEvent("water_alert", "Water reading is outside configured checks. Please inspect and test the water.");
  } else {
    Blynk.virtualWrite(V4, "CHECK PARAMETERS");
  }

  Serial.println("------ Water Quality Monitor ------");
  Serial.print("TDS raw: "); Serial.println(tdsRaw);
  Serial.print("TDS estimate: "); Serial.print(tdsPpmEstimate); Serial.println(" ppm (uncalibrated)");
  Serial.print("Turbidity raw ADC: "); Serial.println(turbRaw);
  Serial.print("Turbidity voltage: "); Serial.print(turbVoltage, 3); Serial.println(" V");
  Serial.print("pH estimate: "); Serial.println(phEstimate, 2);
  Serial.print("Temperature: ");
  if (tempValid) {
    Serial.print(temperatureC, 2); Serial.println(" C");
  } else {
    Serial.println("INVALID / SENSOR DISCONNECTED");
  }
  Serial.println("Note: readings do not certify drinking-water safety.");
  Serial.println("-----------------------------------");
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  tempSensor.begin();

  Serial.println("Connecting to Wi-Fi and Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(3000L, readSensorsAndSend);
}

void loop() {
  Blynk.run();
  timer.run();
}
