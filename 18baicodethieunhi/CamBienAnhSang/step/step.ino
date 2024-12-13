#include <Stepper.h>

// Định nghĩa chân kết nối
const int ldrPin = A0;
int ldrVal;

// Cài đặt động cơ bước
const int stepsPerRevolution = 2048;  // Số bước cho 1 vòng quay
const int stepAngle1 = 90;            // Góc quay 90 độ
const int stepAngle2 = 180;           // Góc quay 180 độ

// Map góc quay sang số bước
const int stepCount1 = map(stepAngle1, 0, 360, 0, stepsPerRevolution);
const int stepCount2 = map(stepAngle2, 0, 360, 0, stepsPerRevolution);

// Khởi tạo đối tượng động cơ bước
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
//in1-8, in2-9,in3-10,in4-11
void setup() {
  myStepper.setSpeed(15);        // Đặt tốc độ động cơ bước (rpm)
  Serial.begin(9600);            // Khởi động giao tiếp Serial để debug
}

void loop() {
  ldrVal = analogRead(ldrPin);

  // In giá trị joystick để debug
  Serial.println(ldrVal);
  if (ldrVal < 877) {  // Gạt joystick lên
    myStepper.step(stepCount2);   // Quay thuận 180 độ
    delay(500);
    return;
  } else {  // Gạt joystick xuống
    myStepper.step(-stepCount2);  // Quay ngược 180 độ
    delay(500);
    return;
  }

  

  delay(100);  // Thời gian nghỉ để giảm tải xử lý
}