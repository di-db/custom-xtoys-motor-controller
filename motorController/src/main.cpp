#include <Arduino.h>
#include <Wire.h>


#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif


#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <time.h>


// WiFi settings
const char *ssid = "MY WIFI ID";             // Replace with your WiFi name
const char *password = "MY WIFI PASSWORD";   // Replace with your WiFi password


// MQTT Broker settings
const char *mqtt_broker = "broker.emqx.io";  // EMQX broker endpoint
const char *mqtt_topic = "MY_TOPIC";     // MQTT topic
const char *mqtt_username = "emqx";  // MQTT username for authentication
const char *mqtt_password = "public";  // MQTT password for authentication
const int mqtt_port = 1883;  // MQTT port (TCP)


WiFiClient espClient;
PubSubClient mqtt_client(espClient);


void connectToWiFi();


void connectToMQTTBroker();


void mqttCallback(char *topic, byte *payload, unsigned int length);


void setup() {
    Serial.begin(115200);
    connectToWiFi();
    analogWriteFreq(20000);
    Serial.begin(9600);
    Serial.println("Hello world!");
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTTBroker();
}


void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to the WiFi network");
}


void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp8266-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_topic);
            // Publish message upon successful connection
            mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP8266 ^^");
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}


void mqttCallback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);
  serializeJsonPretty(doc, Serial);
  int speed = map(doc["speed"], 0, 100, 7, 255);
  Serial.println();
  Serial.println(speed);
  Serial.println();
  if (doc["channel"] == 1) {
      analogWrite(4, speed);
  } else if (doc["channel"] == 2) {
      analogWrite(5, speed);
  }
  else {
      analogWrite(4, 0);
      analogWrite(5, 0);
  }
}


void loop() {
    if (!mqtt_client.connected()) {
        connectToMQTTBroker();
    }
    mqtt_client.loop();
}
