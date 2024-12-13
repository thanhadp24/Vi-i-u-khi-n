#include <Servo.h>

Servo myServo; // Tạo đối tượng servo

int servoPin = 9; // Chân điều khiển servo
const int trigPin = 5; // 
const int echoPin = 3; // 
long duration; // Định nghĩa biến thời gian
int distance; // Định nghĩa biến khoảng cách

void setup() {
  myServo.attach(servoPin);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance Measured = "); 
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 10){
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

  delay(1000); // Tạm dừng 1 giây giữa mỗi lần đo
}
