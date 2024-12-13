#include "Servo.h"

Servo myServo;

int gasPin = A0;
int servoPin = 9;
int val;

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  val = analogRead(gasPin);
  Serial.println(val);

  if (val < 600){
    myServo.write(90);
    delay(250);
    myServo.write(0);
    delay(250);
  }
  else {
    myServo.write(180);
    delay(250);
    myServo.write(0);
    delay(250);
  }
  delay(500);
}