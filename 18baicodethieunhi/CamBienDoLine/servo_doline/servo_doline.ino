#include "Servo.h"

Servo myServo;

const int line_d_pin = 7;
const int line_a_pin = A0;
const int servoPin = 9;

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);
  pinMode(line_d_pin, INPUT);
}

void loop() {
  int d_value = digitalRead(line_d_pin);
  int a_value = analogRead(line_a_pin);

  Serial.println(d_value);
  Serial.println(a_value);
  Serial.println();

  if (a_value < 600){
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