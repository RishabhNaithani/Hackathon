#define BLYNK_TEMPLATE_ID "TMPL3axdPFNFn"
#define BLYNK_TEMPLATE_NAME "Hackathon"
#define BLYNK_AUTH_TOKEN "EclwhdajzhX6031UkKJYoyXYqlZxOEvr"
#include <DFRobot_DHT11.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
const char* thingSpeakApiKey = "PHPHVWN1L6AWWTVO"; 
const char* thingSpeakAddress = "api.thingspeak.com";
DFRobot_DHT11 DHT;  
char ssid[] = "Blynk";
char pass[] = "#blynk123";
int soilpin = A0;  
int motorpin = 12;

#define DHT11_PIN 13

void setup()
{   
    Serial.begin(115200);
     WiFi.begin(ssid,pass);
  
    while (WiFi.status() != WL_CONNECTED){
    Serial.println("trying to connect....");
    delay(200);
  };
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(motorpin,OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{
  digitalWrite(motorpin,LOW);
  Blynk.run();
  DHT.read(DHT11_PIN);
  Serial.print("temp:");
  int temp = DHT.temperature;
  delay(1000);
  Serial.print(temp);
  Blynk.virtualWrite(V0,temp);
  Serial.print(" humi:");
  int humi = DHT.humidity;
  delay(1000);
  Serial.println(humi);
   Blynk.virtualWrite(V1,humi);
   int temp_threshold = Blynk.virtualRead(V3);
  int humi_threshold = Blynk.virtualRead(V4);
  int mp_threshold = Blynk.virtualRead(V5);

  // high humidity warning
if(humi>50){
    Serial.print("HIGH humidity detected");
    Blynk.logEvent("highhumidity", "High humidity detected!");
  }
  else{digitalWrite(motorpin2,HIGH);}
  
  //high temp warning
    if(35<temp)
  {
    
    Serial.print("  temperature is too high!");
    Blynk.logEvent("highTemp", "High temperature detected!");
     digitalWrite(motorpin2,HIGH);
    }

   int moisture_percentage = ( 100.00 - ( (analogRead(soilpin)/1023.00) * 100.00 ) );

  Serial.print("Soil Moisture(in Percentage) = ");
  Serial.print(moisture_percentage);
  Blynk.virtualWrite(V2,moisture_percentage);
  Serial.println("%");

    //motor
    if(moisture_percentage<40){
      digitalWrite(motorpin,HIGH);
      }
      else{
        digitalWrite(motorpin,LOW);
      }
      sendToThink(temp, moisture_percentage, humi);
}
void sendToThink(int temp, int mp, int humi){
   ThingSpeak.setField(1, humi);
  ThingSpeak.setField(2, temp);
  ThingSpeak.setField(2, mp);
  delay(60000);
}