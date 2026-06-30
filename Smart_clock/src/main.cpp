#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
SCREEN_WIDTH,
SCREEN_HEIGHT,
&Wire,
-1
);

// RTC object
RTC_DS1307 rtc;

// Pin definitions
#define BUZZER_PIN 25

#define BTN_SET   15
#define BTN_UP    4
#define BTN_NEXT  16

// Alarm variables
int alarmHour = 7;
int alarmMinute = 30;

bool alarmActive = false;

// Alarm setting mode
bool settingMode = false;
bool settingHour = true;

int tempHour = 7;
int tempMinute = 30;

void setup() {

Serial.begin(115200);

// Initialize I2C
Wire.begin(21, 22);

// Configure pins
pinMode(BUZZER_PIN, OUTPUT);

pinMode(BTN_SET, INPUT_PULLUP);
pinMode(BTN_UP, INPUT_PULLUP);
pinMode(BTN_NEXT, INPUT_PULLUP);

noTone(BUZZER_PIN);

// Initialize OLED
if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
Serial.println("OLED initialization failed");
while (true);
}

display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.display();

// Initialize RTC
if (!rtc.begin()) {
Serial.println("RTC initialization failed");
while (true);
}

// Set RTC time if RTC is not running
if (!rtc.isrunning()) {
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

Serial.println("Smart Clock Started");
}

void loop() {

DateTime now = rtc.now();

// ================= Enter / Exit Setting Mode =================
if (!alarmActive && digitalRead(BTN_SET) == LOW) {

settingMode = !settingMode;

if (settingMode) {
  tempHour = alarmHour;
  tempMinute = alarmMinute;
}

delay(300);


}

// ================= Alarm Setting Mode =================
if (settingMode) {


// Increase value
if (digitalRead(BTN_UP) == LOW) {

  if (settingHour)
    tempHour = (tempHour + 1) % 24;
  else
    tempMinute = (tempMinute + 1) % 60;

  delay(250);
}

// Move to next field or save
if (digitalRead(BTN_NEXT) == LOW) {

  if (settingHour) {

    settingHour = false;
  }
  else {

    alarmHour = tempHour;
    alarmMinute = tempMinute;

    settingHour = true;
    settingMode = false;

    Serial.println("Alarm Saved");
  }

  delay(300);
}


}

// ================= Trigger Alarm =================
if (now.hour() == alarmHour &&
now.minute() == alarmMinute &&
now.second() == 0 &&
!alarmActive) {


alarmActive = true;
Serial.println("Alarm Started");


}

// ================= Alarm Operation =================
if (alarmActive) {


// Generate sound continuously
tone(BUZZER_PIN, 1000);

if (digitalRead(BTN_SET) == LOW) {

  unsigned long pressStart = millis();

  // Wait until button released
  while (digitalRead(BTN_SET) == LOW) {
    delay(10);
  }

  unsigned long pressDuration = millis() - pressStart;

  // Long press -> Stop alarm permanently
  if (pressDuration >= 3000) {

    alarmActive = false;
    noTone(BUZZER_PIN);

    Serial.println("Alarm Stopped");
  }

  // Short press -> Snooze
  else {

    DateTime snoozeTime = now + TimeSpan(0, 0, 1, 0);

    alarmHour = snoozeTime.hour();
    alarmMinute = snoozeTime.minute();

    alarmActive = false;
    noTone(BUZZER_PIN);

    Serial.print("Alarm Snoozed To ");

    if (alarmHour < 10) Serial.print("0");
    Serial.print(alarmHour);

    Serial.print(":");

    if (alarmMinute < 10) Serial.print("0");
    Serial.println(alarmMinute);
  }

  delay(500);
}


}
else {


noTone(BUZZER_PIN);


}

// ================= OLED Display =================
display.clearDisplay();

if (settingMode) {

display.setTextSize(1);

display.setCursor(0, 0);
display.println("SET ALARM");

display.setCursor(0, 20);
display.print("Hour: ");
display.println(tempHour);

display.setCursor(0, 40);
display.print("Minute: ");
display.println(tempMinute);

display.setCursor(0, 55);

if (settingHour)
  display.println("Editing Hour");
else
  display.println("Editing Minute");

}
else {

// Display current time
display.setTextSize(2);
display.setCursor(0, 0);

if (now.hour() < 10) display.print("0");
display.print(now.hour());

display.print(":");

if (now.minute() < 10) display.print("0");
display.print(now.minute());

display.print(":");

if (now.second() < 10) display.print("0");
display.print(now.second());

// Display date
display.setTextSize(1);

display.setCursor(18, 35);

if (now.day() < 10) display.print("0");
display.print(now.day());

display.print("/");

if (now.month() < 10) display.print("0");
display.print(now.month());

display.print("/");

display.print(now.year());

// Display alarm
display.setCursor(0, 55);

display.print("Alarm: ");

if (alarmHour < 10) display.print("0");
display.print(alarmHour);

display.print(":");

if (alarmMinute < 10) display.print("0");
display.print(alarmMinute);


}

display.display();

delay(100);
}
