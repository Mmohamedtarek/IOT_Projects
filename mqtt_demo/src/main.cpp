#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// HiveMQ Cloud
const char* mqtt_server =
"258f0d6b2e9f4e31b9efdf26707b955b.s1.eu.hivemq.cloud";

const int mqtt_port = 8883;

const char* mqtt_username = "mohamed_tarek";
const char* mqtt_password = "Strongpassword#123";

const char* topic = "mohamed/test";

WiFiClientSecure espClient;
PubSubClient client(espClient);


// تستدعى عند وصول رسالة
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived on topic: ");
  Serial.println(topic);

  Serial.print("Message: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("----------------------");
}


void connectMQTT() {

  while (!client.connected()) {

    Serial.println("Connecting to HiveMQ Cloud...");

    String clientId = "ESP32Client-";
    clientId += String(random(1000));

    if (client.connect(
          clientId.c_str(),
          mqtt_username,
          mqtt_password)) {

      Serial.println("Connected to HiveMQ Cloud!");

      client.subscribe(topic);

      client.publish(topic,
                     "Hello from ESP32 via HiveMQ Cloud");

    } else {

      Serial.print("Failed, state = ");
      Serial.println(client.state());

      delay(2000);
    }
  }
}


void setup() {

  Serial.begin(115200);

  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");

  // للتجارب فقط
  espClient.setInsecure();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  connectMQTT();
}


void loop() {

  if (!client.connected()) {
    connectMQTT();
  }

  client.loop();
}