#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define BUZZER_PIN 26
#define LED_PIN 27
#define BUTTON_PIN 15

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
SCREEN_WIDTH,
SCREEN_HEIGHT,
&Wire,
-1
);

bool systemEnabled = true;
bool lastButtonState = HIGH;

void setup() {

Serial.begin(115200);

pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);

pinMode(BUZZER_PIN, OUTPUT);
pinMode(LED_PIN, OUTPUT);

pinMode(BUTTON_PIN, INPUT_PULLUP);

if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
Serial.println("OLED Failed!");
while (true);
}

display.clearDisplay();
display.setTextColor(SSD1306_WHITE);

Serial.println("Distance Logger Started");
}

void loop() {

// ===== Button Handling =====
bool currentButtonState = digitalRead(BUTTON_PIN);

if (lastButtonState == HIGH &&
currentButtonState == LOW) {

systemEnabled = !systemEnabled;

Serial.print("System: ");
Serial.println(systemEnabled ? "ON" : "OFF");

delay(300); // debounce


}

lastButtonState = currentButtonState;

// ===== If System OFF =====
if (!systemEnabled) {


digitalWrite(LED_PIN, LOW);
noTone(BUZZER_PIN);

display.clearDisplay();
display.setTextSize(2);
display.setCursor(10, 20);
display.println("SYSTEM");
display.setCursor(35, 45);
display.println("OFF");
display.display();

delay(100);
return;


}

// ===== Ultrasonic Measurement =====

digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);

digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);

digitalWrite(TRIG_PIN, LOW);

long duration = pulseIn(ECHO_PIN, HIGH);

float distance = duration * 0.034 / 2;

Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");

// ===== Alarm =====
if (distance < 20) {


digitalWrite(LED_PIN, HIGH);

// Generate alarm tone (2 KHz)
tone(BUZZER_PIN, 2000);


}
else {


digitalWrite(LED_PIN, LOW);

// Stop buzzer sound
noTone(BUZZER_PIN);


}

// ===== OLED =====

display.clearDisplay();

display.setTextSize(2);
display.setCursor(0, 0);
display.print(distance, 1);
display.println("cm");

display.setTextSize(1);
display.setCursor(0, 40);

if (distance < 20)
display.println("Object Nearby!");
else
display.println("Area Clear");

display.display();

delay(500);
}
