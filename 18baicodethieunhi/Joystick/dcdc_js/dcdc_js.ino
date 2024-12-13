
const int JoyStick_X = A0;  // Chân kết nối trục X của joystick
const int JoyStick_Y = A1;  // Chân kết nối trục Y của joystick
const int button = 3;       // Chân nút nhấn (không được sử dụng ở đây)

int in1 = 7;
int in2 = 6;
int e = 9;

void setup() {
  pinMode(button, INPUT_PULLUP);  // Thiết lập nút nhấn (nếu cần sử dụng sau này)
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(e, OUTPUT);    // Đặt tốc độ động cơ bước (rpm)
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
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(e, 255);
    delay(500);
  } else if (y > 900) {  // Gạt joystick xuống
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(e, 255);
    delay(500);
  }
  // Điều khiển động cơ dựa vào hướng joystick
  else if (y >= 200 && y <= 900 && x < 400) {  // Gạt joystick sang trái
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(e, 255);
    delay(500);
  } else if (y >= 200 && y <= 900 && x > 600) {  // Gạt joystick sang phải
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(e, 255);
    delay(500);
    return;
  }
  delay(100);  // Thời gian nghỉ để giảm tải xử lý
}