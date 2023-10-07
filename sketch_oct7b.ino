#include <ESP8266WiFi.h>
#include <DFRobot_DHT11.h>
#include <ThingSpeak.h>

DFRobot_DHT11 DHT;
#define DHTPIN 0

const char* ssid = "Blynk";             // Your WiFi SSID
const char* password = "#blynk123";     // Your WiFi password
const char* thingSpeakApiKey = "PHPHVWN1L6AWWTVO"; // Your ThingSpeak API Key
const char* thingSpeakAddress = "api.thingspeak.com";

void setup() {
  Serial.begin(115200);
   Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
}

void loop() {
  DHT.read(DHTPIN);
      float h = DHT.humidity;
      float t = DHT.temperature;

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C | Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  sendDataToThingSpeak(t, h);

  delay(60000);
}


void sendDataToThingSpeak(float temperature, float humidity) {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  
  int status = ThingSpeak.writeFields(2295350, thingSpeakApiKey);
  
  if (status == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.println("Failed to send data to ThingSpeak. Status code: " + String(status));
  }
}
