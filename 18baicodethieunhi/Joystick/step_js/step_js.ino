#include <Stepper.h>

// Định nghĩa chân kết nối
const int JoyStick_X = A0;  // Chân kết nối trục X của joystick
const int JoyStick_Y = A1;  // Chân kết nối trục Y của joystick
const int button = 3;       // Chân nút nhấn (không được sử dụng ở đây)

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
  pinMode(button, INPUT_PULLUP);  // Thiết lập nút nhấn (nếu cần sử dụng sau này)
  myStepper.setSpeed(15);        // Đặt tốc độ động cơ bước (rpm)
  Serial.begin(9600);            // Khởi động giao tiếp Serial để debug
}

void loop() {
  int x = analogRead(JoyStick_X);  // Đọc giá trị trục X của joystick
  int y = analogRead(JoyStick_Y);  // Đọc giá trị trục Y của joystick

  // In giá trị joystick để debug
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.println(y);
  if (y < 200) {  // Gạt joystick lên
    myStepper.step(stepCount2);   // Quay thuận 180 độ
    delay(500);
    return;
  } else if (y > 900) {  // Gạt joystick xuống
    myStepper.step(-stepCount2);  // Quay ngược 180 độ
    delay(500);
    return;
  }
  // Điều khiển động cơ dựa vào hướng joystick
  else if (y >= 200 && y <= 900 && x < 400) {  // Gạt joystick sang trái
    myStepper.step(-stepCount1);  // Quay ngược 90 độ
    delay(500);
    return;
  } else if (y >= 200 && y <= 900 && x > 600) {  // Gạt joystick sang phải
    myStepper.step(stepCount1);   // Quay thuận 90 độ
    delay(500);
    return;
  }

  

  delay(100);  // Thời gian nghỉ để giảm tải xử lý
}