#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- Pins ----------------
#define DHT_PIN     4
#define DHT_TYPE    DHT22

#define POT_PIN     34
#define LED_PIN     25

// ---------------- OLED ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
SCREEN_WIDTH,
SCREEN_HEIGHT,
&Wire,
-1
);

// ---------------- WiFi ----------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

WebServer server(80);

// ---------------- DHT ----------------
DHT dht(DHT_PIN, DHT_TYPE);

// ---------------- Variables ----------------
int ledBrightness = 0;

// ---------------- HTML Page ----------------
String createWebPage(float temp, float hum, int potValue) {

String page = R"rawliteral(

<!DOCTYPE html>

<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
  font-family: Arial;
  text-align:center;
  background:#f2f2f2;
}

.card{
  background:white;
  padding:20px;
  border-radius:15px;
  width:300px;
  margin:auto;
  margin-top:40px;
  box-shadow:0 0 10px gray;
}

.slider{
  width:100%;
}

</style>

</head>

<body>

<div class='card'>

<h2>Mini IoT Dashboard</h2>

<p>Temperature: )rawliteral";

page += String(temp, 1);

page += R"rawliteral( °C</p>

<p>Humidity: )rawliteral";

page += String(hum, 1);

page += R"rawliteral( %</p>

<p>Potentiometer: )rawliteral";

page += String(potValue);

page += R"rawliteral( %</p>

<p>LED Brightness: <span id='value'>)rawliteral";

page += String(ledBrightness);

page += R"rawliteral(</span>%</p>

<input type='range'
min='0'
max='100'
value=')rawliteral";

page += String(ledBrightness);

page += R"rawliteral('
class='slider'
onchange='updateLED(this.value)'>

</div>

<script>

function updateLED(value){
 fetch("/set?value="+value);
 document.getElementById("value").innerHTML=value;
}

setInterval(function(){
 location.reload();
},5000);

</script>

</body>
</html>
)rawliteral";

return page;
}

// ---------------- Root Page ----------------
void handleRoot() {

float temp = dht.readTemperature();
float hum = dht.readHumidity();

int potRaw = analogRead(POT_PIN);
int potPercent = map(potRaw, 0, 4095, 0, 100);

server.send(
200,
"text/html",
createWebPage(temp, hum, potPercent)
);
}

// ---------------- LED Control ----------------
void handleLED() {

if (server.hasArg("value")) {

ledBrightness = server.arg("value").toInt();

int pwm = map(ledBrightness, 0, 100, 0, 255);

analogWrite(LED_PIN, pwm);


}

server.send(200, "text/plain", "OK");
}

// ---------------- Setup ----------------
void setup() {

Serial.begin(115200);

pinMode(LED_PIN, OUTPUT);

dht.begin();

Wire.begin(21,22);

if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
while(true);
}

display.setTextColor(SSD1306_WHITE);

// WiFi connection
WiFi.begin(ssid, password);

Serial.print("Connecting");

while (WiFi.status() != WL_CONNECTED) {

delay(500);
Serial.print(".");


}

Serial.println();
Serial.println("Connected");

Serial.print("IP Address: ");
Serial.println(WiFi.localIP());

// Web server routes
server.on("/", handleRoot);
server.on("/set", handleLED);

server.begin();

Serial.println("Web Server Started");
}

// ---------------- Loop ----------------
void loop() {

server.handleClient();

float temp = dht.readTemperature();
float hum = dht.readHumidity();

int potRaw = analogRead(POT_PIN);
int potPercent = map(potRaw, 0, 4095, 0, 100);

// OLED display
display.clearDisplay();

display.setTextSize(1);

display.setCursor(0,0);
display.print("Temp: ");
display.print(temp,1);
display.println(" C");

display.setCursor(0,16);
display.print("Hum: ");
display.print(hum,1);
display.println("%");

display.setCursor(0,32);
display.print("Pot: ");
display.print(potPercent);
display.println("%");

display.setCursor(0,48);
display.print("LED: ");
display.print(ledBrightness);
display.println("%");

display.display();

delay(200);
}
