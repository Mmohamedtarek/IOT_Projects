#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= DHT22 =================
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ================= Push Button =================
#define BUTTON_PIN 15

bool showFahrenheit = false;
bool lastButtonState = HIGH; // INPUT_PULLUP

void setup() {

  Serial.begin(115200);

  // DHT22
  dht.begin();

  // Push Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED initialization failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  Serial.println("Smart Weather Station Started");
}

void loop() {

  // ================= Button =================
  bool currentButtonState = digitalRead(BUTTON_PIN);

  // Detect button press
  if (currentButtonState == LOW && lastButtonState == HIGH) {

    showFahrenheit = !showFahrenheit;

    Serial.println("Button Pressed");

    delay(200); // Debounce
  }

  lastButtonState = currentButtonState;

  // ================= Read DHT22 =================
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();
  float tempF = dht.readTemperature(true);

  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("Failed to read from DHT22");
    return;
  }

  // ================= Weather Status =================
  String status;

  if (humidity < 30)
    status = "DRY";
  else if (humidity <= 60)
    status = "OK";
  else
    status = "HUMID";

  // ================= OLED Display =================
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 0);

  if (showFahrenheit) {
    display.print(tempF, 1);
    display.println(" F");
  } else {
    display.print(tempC, 1);
    display.println(" C");
  }

  display.setTextSize(1);

  display.setCursor(0, 30);
  display.print("Humidity: ");
  display.print(humidity, 1);
  display.println("%");

  display.setCursor(0, 50);
  display.print("Status: ");
  display.println(status);

  display.display();

  // ================= Serial Monitor =================
  Serial.print("Temperature: ");

  if (showFahrenheit)
    Serial.print(tempF);
  else
    Serial.print(tempC);

  Serial.print(showFahrenheit ? " F" : " C");

  Serial.print(" | Humidity: ");
  Serial.print(humidity);

  Serial.print("% | Status: ");
  Serial.println(status);

  delay(500);
}