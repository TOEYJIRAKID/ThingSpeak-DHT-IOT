#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

const char* ssid = "MAIBOK";
const char* password = "1223334444";

unsigned long channelID = 2339338;
const char* writeKey = "N07WIIG559VLKB3C";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

WiFiClient client;
DHT dht(D4, DHT11);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(5000);
      }
      Serial.println("Connected.");
    }

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    ThingSpeak.setField(1, humidity);
    ThingSpeak.setField(2, temperature);

    int X = ThingSpeak.writeFields(channelID, writeKey);
    if (X == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(X));
    }

    lastTime = millis();
  }
}