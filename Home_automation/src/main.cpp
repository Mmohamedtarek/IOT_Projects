#include <Arduino.h>
#include <Wire.h>
#include <Preferences.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON1_PIN 15
#define BUTTON2_PIN 4

#define RELAY1_PIN 26
#define RELAY2_PIN 25

#define LED1_PIN 27
#define LED2_PIN 14

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Preferences prefs;

Adafruit_SSD1306 display(
SCREEN_WIDTH,
SCREEN_HEIGHT,
&Wire,
-1
);

bool device1 = false; // Light
bool device2 = false; // Fan

bool lastButton1 = HIGH;
bool lastButton2 = HIGH;

// =========================================

void updateOutputs() {

digitalWrite(RELAY1_PIN, device1);
digitalWrite(LED1_PIN, device1);

digitalWrite(RELAY2_PIN, device2);
digitalWrite(LED2_PIN, device2);
}

// =========================================

void saveStates() {

prefs.putBool("light", device1);
prefs.putBool("fan", device2);

Serial.println("States Saved");
}

// =========================================

void updateDisplay() {

display.clearDisplay();

display.setTextColor(SSD1306_WHITE);
display.setTextSize(1);

display.setCursor(0, 0);
display.println("HOME CONTROL");

display.setCursor(0, 20);
display.print("Light: ");
display.println(device1 ? "ON" : "OFF");

display.setCursor(0, 40);
display.print("Fan: ");
display.println(device2 ? "ON" : "OFF");

display.display();
}

// =========================================

void setup() {

Serial.begin(115200);

Wire.begin(21, 22);

pinMode(BUTTON1_PIN, INPUT_PULLUP);
pinMode(BUTTON2_PIN, INPUT_PULLUP);

pinMode(RELAY1_PIN, OUTPUT);
pinMode(RELAY2_PIN, OUTPUT);

pinMode(LED1_PIN, OUTPUT);
pinMode(LED2_PIN, OUTPUT);

// Open NVS storage
prefs.begin("home_ctrl", false);

// Read saved states
device1 = prefs.getBool("light", false);
device2 = prefs.getBool("fan", false);

Serial.print("Saved Light State: ");
Serial.println(device1);

Serial.print("Saved Fan State: ");
Serial.println(device2);

// Apply saved states
updateOutputs();

// Initialize OLED
if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
Serial.println("OLED Failed");
while (true);
}

updateDisplay();

Serial.println("Home Automation Started");
}

// =========================================

void loop() {

bool currentButton1 = digitalRead(BUTTON1_PIN);
bool currentButton2 = digitalRead(BUTTON2_PIN);

// Toggle Light
if (lastButton1 == HIGH &&
currentButton1 == LOW) {

device1 = !device1;

updateOutputs();
saveStates();
updateDisplay();

Serial.print("Light: ");
Serial.println(device1 ? "ON" : "OFF");

delay(300);


}

// Toggle Fan
if (lastButton2 == HIGH &&
currentButton2 == LOW) {


device2 = !device2;

updateOutputs();
saveStates();
updateDisplay();

Serial.print("Fan: ");
Serial.println(device2 ? "ON" : "OFF");

delay(300);

}

lastButton1 = currentButton1;
lastButton2 = currentButton2;
}
