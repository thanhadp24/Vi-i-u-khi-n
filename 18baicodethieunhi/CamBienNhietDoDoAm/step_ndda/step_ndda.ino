#include "DHT.h"
#include <Stepper.h>

const int DHT_PIN = 4;
const int DHT_TYPE = DHT11;

// Cài đặt động cơ bước
const int stepsPerRevolution = 2048;  // Số bước cho 1 vòng quay
const int stepAngle1 = 90;            // Góc quay 90 độ
const int stepAngle2 = 180;           // Góc quay 180 độ

// Map góc quay sang số bước
const int stepCount1 = map(stepAngle1, 0, 360, 0, stepsPerRevolution);
const int stepCount2 = map(stepAngle2, 0, 360, 0, stepsPerRevolution);

// Khởi tạo đối tượng động cơ bước
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

DHT dht(DHT_PIN, DHT_TYPE);

void setup(){
  myStepper.setSpeed(15);   
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
  if (t < 27) { 
    myStepper.step(stepCount2);   // Quay thuận 180 độ
    delay(500);
    return;
  } else {  
    myStepper.step(-stepCount2);  // Quay ngược 180 độ
    delay(500);
    return;
  }

  delay(2000);
}