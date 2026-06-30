#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// ================= DHT22 =================
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ================= Gas Sensor =================
#define GAS_PIN 34

// ================= Outputs =================
#define BUZZER_PIN 26
#define LED_PIN 27

void setup() {

  Serial.begin(115200);

  dht.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  Serial.println("Air Quality Monitor Started");
}

void loop() {

  // Read DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read Gas Sensor
  int gasValue = analogRead(GAS_PIN);

  // Check DHT sensor
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read DHT22");
    return;
  }

  // Determine Air Quality
  String airStatus;

  if (gasValue < 1000) {
    airStatus = "GOOD";
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (gasValue < 2500) {
    airStatus = "MODERATE";
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else {
    airStatus = "BAD";

    digitalWrite(LED_PIN, HIGH);

    // Buzzer alarm
    tone(BUZZER_PIN, 1000);
    delay(300);
    noTone(BUZZER_PIN);
  }

  // ================= OLED =================
  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("Air: ");
  display.println(airStatus);

  display.setCursor(0, 16);
  display.print("Gas: ");
  display.println(gasValue);

  display.setCursor(0, 32);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 48);
  display.print("Hum: ");
  display.print(humidity, 1);
  display.println(" %");

  display.display();

  // ================= Serial Monitor =================
  Serial.print("Gas: ");
  Serial.print(gasValue);

  Serial.print(" | Air: ");
  Serial.print(airStatus);

  Serial.print(" | Temp: ");
  Serial.print(temperature);

  Serial.print(" C | Humidity: ");
  Serial.print(humidity);

  Serial.println("%");

  delay(2000);
}