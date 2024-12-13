#include <Servo.h>

Servo myServo; // Tạo đối tượng servo
int ldrPin = A0; // Chân cảm biến ánh sáng
int servoPin = 9; // Chân điều khiển servo
int ldrValue; // Biến lưu giá trị ánh sáng

void setup() {
  myServo.attach(servoPin); // Kết nối servo với chân điều khiển
  Serial.begin(9600); // Khởi tạo giao tiếp Serial để debug
}

void loop() {
  ldrValue = analogRead(ldrPin); // Đọc giá trị từ cảm biến ánh sáng
  Serial.println(ldrValue); // In giá trị ánh sáng để debug

  if (ldrValue < 870){
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


