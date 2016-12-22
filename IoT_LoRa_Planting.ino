#include "MOSTLora.h"
#include "MLPacketComm.h"
//#include "MLutility.h"
#include <DHT.h>
#define FAN_PORT 10
#define LED_PORT 6
#define loraSerial Serial1

MOSTLora lora;
DHT dht(2, DHT22);
float fTemperature, fHumidity;

int szRece = 0;

const char *thinkSpeakApiKey = "W00UTJRN68Z7HJJN";    // ThingSpeak API-key

void setup() {
#ifdef DEBUG_LORA
  Serial.begin(9600);  // use serial port for log monitor
  Serial.println("--- App: lora_thingspeak ---");  // use serial port
#endif // DEBUG_LORA
  
  lora.begin();
  // custom LoRa config by your environment setting
  lora.writeConfig(915555, 0, 0, 7, 5);
  lora.setMode(E_LORA_NORMAL);
  pinMode(LED_PORT, OUTPUT);
  pinMode(FAN_PORT, OUTPUT);

  delay(1000);
  
  // init sensor for humidity & temperature
  dht.begin();
  int i = 0;
  boolean bReadDHT = false;
}

void loop() {
  if (lora.available()) {
    szRece = lora.receData();
    if (lora.parsePacket() == CMD_REQ_DATA) {
      lora.sendPacketThingSpeak(thinkSpeakApiKey, fHumidity, fTemperature, 33, 46, 59, 60, 70, 85);
    }
  }
  
  if(szRece == 1){
     digitalWrite(FAN_PORT, HIGH);
     Serial.println("FAN ON");
  }
  if(szRece == 3){
     digitalWrite(FAN_PORT, LOW);
     Serial.println("FAN OFF");
  }
  if(szRece == 5){
     digitalWrite(LED_PORT, HIGH);
     Serial.println("LIGHT ON");
  }
  if(szRece == 7){
     digitalWrite(LED_PORT, LOW);
     Serial.println("LIGHT OFF");
  }
  delay(100);
}
