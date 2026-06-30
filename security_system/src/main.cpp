#include <Arduino.h>

#define PIR_PIN      13
#define BUZZER_PIN   26
#define LED_PIN      27
#define BUTTON_PIN   15

bool systemArmed = false;
bool lastButtonState = HIGH;

void setup() {

Serial.begin(115200);

pinMode(PIR_PIN, INPUT);

pinMode(BUZZER_PIN, OUTPUT);
pinMode(LED_PIN, OUTPUT);

// Internal Pull-Up
pinMode(BUTTON_PIN, INPUT_PULLUP);

Serial.println("Security System Ready");
Serial.println("Press button to ARM/DISARM");
}

void loop() {

// ===== Button Handling =====

bool currentButtonState = digitalRead(BUTTON_PIN);

if (lastButtonState == HIGH &&
currentButtonState == LOW) {


systemArmed = !systemArmed;

if (systemArmed)
  Serial.println("System ARMED");
else
  Serial.println("System DISARMED");

delay(300); // debounce


}

lastButtonState = currentButtonState;

// ===== Motion Detection =====

if (systemArmed) {


int motion = digitalRead(PIR_PIN);

if (motion == HIGH) {

  Serial.println("Motion Detected!");

  digitalWrite(LED_PIN, HIGH);

  // Generate alarm sound (2 KHz)
  tone(BUZZER_PIN, 2000);

} else {

  digitalWrite(LED_PIN, LOW);

  // Stop buzzer sound
  noTone(BUZZER_PIN);
}


} else {


digitalWrite(LED_PIN, LOW);

// Stop buzzer when system is disarmed
noTone(BUZZER_PIN);


}

delay(100);
}
