#include <IRremote.hpp>
#include <Servo.h>
Servo myServo;

const int IR_RECEIVE_PIN = 2;
// const int LED_Pin = 9;

volatile bool ledShouldBeOn = false; // Biến để lựu trạng thái của LED (dựa trên tín hiệu nhận được)a

void setup() {
  myServo.attach(9);
  Serial.begin(9600); // Khởi tạo kết nối Serial
  // pinMode(LED_Pin, OUTPUT); // Thiết lập LED_Pin là ngõ ra
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Bắt đầu bộ nhận IR
}

// Pin để nhận tín hiệu IR
// Pin để điều khiển đèn LED

void loop() {


  // Resume receiving IR signals
  if (IrReceiver.decode()) {
  Serial.println("Receive Signal!");
  handleReceivedIRData(); // Gọi hàm xử lý dữ liệu khi nhận được tín hiệu IR
  delay(5);
  IrReceiver.resume(); // Tiếp tục nhận tín hiệu IR
  }
  delay(500);
}

void handleReceivedIRData() {
  Serial.print("Mã IR nhận được: 0x");
  Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
  Serial.print("Giao thức: ");
  Serial.println(IrReceiver.decodedIRData.protocol);

  // Nếu giá trị nhận được là 0x101, bật LED
  if (IrReceiver.decodedIRData.decodedRawData == 0x101){
    myServo.write(90);
    delay(500);
    myServo.write(0);
    delay(500);
  }
  else {
    myServo.write(45);
    delay(500);
    myServo.write(0);
    delay(500);
  }
}