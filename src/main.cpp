#include <Arduino.h>
#include "DHT.h"
#include "Adafruit_Sensor.h"

#define redLed 12
#define greenLed 11

#define gasSensor A0
#define DHTPIN 2
#define DHTTYPE DHT11

String quality =""; 

DHT dht(DHTPIN, DHTTYPE);

typedef struct dataStruct {
  float temp;
  float hud;
  int gasLevel;
} data;

data sensorData;

void setup() {

  pinMode(gasSensor, INPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  dht.begin();
  Serial.begin(9600);
}

void printData(dataStruct data) {
  Serial.print("Temperature: ");
  Serial.print(data.temp);
  Serial.print("°C");
  Serial.print(" - Humidity: ");
  Serial.print(data.hud);
  Serial.println("%");
  Serial.print("Gas level: ");
  Serial.print(data.gasLevel);
  Serial.print(" - ");
  Serial.println(quality);
}

void loop() {
  //read from dht11

  sensorData.temp = dht.readTemperature();
  sensorData.hud = dht.readHumidity();

  if (isnan(sensorData.temp) || isnan(sensorData.hud)) {
    Serial.println("Cannot read data from DHT11.");
    return;
  }

  //read from mq2
  sensorData.gasLevel = analogRead(gasSensor);
  int gasLevel = sensorData.gasLevel;

  if(gasLevel < 225) {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);

    if(gasLevel < 180) quality = " GOOD!";
    else quality = " Poor!";

  } 
  else { // gasLevel >= 225
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);

    if (gasLevel < 350) {
      if (gasLevel < 300) quality = " Very bad!";
      else quality = " You dead!";
    }
    
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
    quality = " Omae wa mou shindeiru."; 
  }

  printData(sensorData);

  delay(1000);
}