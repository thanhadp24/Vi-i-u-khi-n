#include "DHT.h"

int in1 = 7;
int in2 = 6;
int e = 9;

const int DHT_PIN = 4;
const int DHT_TYPE = DHT11;

DHT dht(DHT_PIN, DHT_TYPE);

void setup(){
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(e, OUTPUT);  
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM");
  dht.begin();
  Serial.println("Type\tHumidity (%)\tTemperature (C)");
}

void loop(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)){
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("DHT11 \t");
    Serial.print(h);
    Serial.print("%     \t");
    Serial.print(t);
    Serial.print("C");
    Serial.println();
  }
  if (t < 27){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(e, 255);
    delay(500);
  }

  else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(e, 255);
    delay(500);
  }

  delay(2000);
}