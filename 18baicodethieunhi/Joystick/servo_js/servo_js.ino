#include <Servo.h>
Servo myServo; // Tạo đối tượng servo

int servoPin = 9; // Chân điều khiển servo

// Định nghĩa chân kết nối
const int JoyStick_X = A0;  // Chân kết nối trục X của joystick
const int JoyStick_Y = A1;  // Chân kết nối trục Y của joystick
const int button = 3;       // Chân nút nhấn (không được sử dụng ở đây)

void setup() {
  pinMode(button, INPUT_PULLUP);  // Thiết lập nút nhấn (nếu cần sử dụng sau này)
  myServo.attach(servoPin);
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
    myServo.write(90);
    delay(100);
    myServo.write(0);
    delay(100);
  } else if (y > 900) {  // Gạt joystick xuống
    myServo.write(180);
    delay(100);
    myServo.write(0);
    delay(100);
  }
  // Điều khiển động cơ dựa vào hướng joystick
  else if (y >= 200 && y <= 900 && x < 400) {  // Gạt joystick sang trái
    myServo.write(45);
    delay(100);
    myServo.write(0);
    delay(100);
  } else if (y >= 200 && y <= 900 && x > 600) {  // Gạt joystick sang phải
    myServo.write(135);
    delay(100);
    myServo.write(0);
    delay(100);
  }

  

  delay(200);  // Thời gian nghỉ để giảm tải xử lý
}