#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ================= Pins =================
#define SOIL_PIN   34
#define RELAY_PIN  26

void setup() {

  Serial.begin(115200);

  // Initialize Relay
  pinMode(RELAY_PIN, OUTPUT);

  // Pump OFF at startup
  digitalWrite(RELAY_PIN, LOW);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  Serial.println("Plant Monitor Started");
}

void loop() {

  // Read potentiometer value (0 -> 4095)
  int rawValue = analogRead(SOIL_PIN);

  // Convert ADC value to moisture percentage
  int moisture = map(rawValue, 4095, 0, 0, 100);
  moisture = constrain(moisture, 0, 100);

  // Determine soil condition
  String status;
  bool pumpON = false;

  if (moisture < 30) {
    status = "DRY";
    pumpON = true;
  }
  else if (moisture <= 60) {
    status = "NORMAL";
  }
  else {
    status = "WET";
  }

  // Control relay (pump)
  digitalWrite(RELAY_PIN, pumpON ? HIGH : LOW);

  // ================= OLED =================

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print(moisture);
  display.println("%");

  display.setTextSize(1);

  display.setCursor(0, 28);
  display.print("Soil: ");
  display.println(status);

  display.setCursor(0, 44);
  display.print("Pump: ");
  display.println(pumpON ? "ON" : "OFF");

  display.display();

  // ================= Serial Monitor =================

  Serial.print("Moisture: ");
  Serial.print(moisture);
  Serial.print("% | Status: ");
  Serial.print(status);
  Serial.print(" | Pump: ");
  Serial.println(pumpON ? "ON" : "OFF");

  delay(1000);
}