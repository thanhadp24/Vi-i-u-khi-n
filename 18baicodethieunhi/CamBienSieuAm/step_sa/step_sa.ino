#include <Stepper.h>

const int stepsPerRevolution = 2048;  // Số bước cho 1 vòng quay
const int stepAngle1 = 90;            // Góc quay 90 độ
const int stepAngle2 = 180;           // Góc quay 180 độ

// Map góc quay sang số bước
const int stepCount1 = map(stepAngle1, 0, 360, 0, stepsPerRevolution);
const int stepCount2 = map(stepAngle2, 0, 360, 0, stepsPerRevolution);

// Khởi tạo đối tượng động cơ bước
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); 

const int trigPin = 5; // 
const int echoPin = 3; // 
long duration; // Định nghĩa biến thời gian
int distance; // Định nghĩa biến khoảng cách

void setup() {
  myStepper.setSpeed(15);   
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
    myStepper.step(stepCount2);   // Quay thuận 180 độ
    delay(500);
    return;
  }
  else {
    myStepper.step(-stepCount2);  // Quay ngược 180 độ
    delay(500);
    return;
  }

  delay(100); // Tạm dừng 1 giây giữa mỗi lần đo
}
