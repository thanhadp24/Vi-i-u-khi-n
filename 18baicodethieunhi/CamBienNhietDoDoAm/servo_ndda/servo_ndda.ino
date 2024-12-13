#include <Servo.h>
#include "DHT.h"

const int DHT_PIN = 4;
const int DHT_TYPE = DHT11;

DHT dht(DHT_PIN, DHT_TYPE);

Servo myServo; // Tạo đối tượng servo
int ldrPin = A0; // Chân cảm biến ánh sáng
int servoPin = 9; // Chân điều khiển servo
int ldrValue; // Biến lưu giá trị ánh sáng

void setup() {
  myServo.attach(servoPin); // Kết nối servo với chân điều khiển
  Serial.begin(9600);
  Serial.println("DHT TEST PROGRAM");
  dht.begin();
  Serial.println("Type\tHumidity (%)\tTemperature (C)");
}

void loop() {
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
    myServo.write(90);
    delay(100);
    myServo.write(0);
    delay(100);
  }
  else {
    myServo.write(180);
    delay(100);
    myServo.write(0);
    delay(100);
  }
  delay(200);
}


