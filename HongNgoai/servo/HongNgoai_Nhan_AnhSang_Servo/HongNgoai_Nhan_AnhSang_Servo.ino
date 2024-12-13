#include <IRremote.hpp>
#include <Servo.h>
Servo myServo;

const int IR_RECEIVE_PIN = 3;
// const int LED_Pin = 9;


void setup() {
  myServo.attach(8);
  Serial.begin(9600); // Khởi tạo kết nối Serial
  // pinMode(LED_Pin, OUTPUT); // Thiết lập LED_Pin là ngõ ra
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Bắt đầu bộ nhận IR
}

// Pin để nhận tín hiệu IR
// Pin để điều khiển đèn LED

void loop() {

  if (IrReceiver.decode()) {
  Serial.println("Receive Signal!");
  handleReceivedIRData(); // Gọi hàm xử lý dữ liệu khi nhận được tín hiệu IR
  delay(5);
  IrReceiver.resume(); // Tiếp tục nhận tín hiệu IR
  }
  delay(100);
  }

  void handleReceivedIRData() {
  Serial.print("Mã IR nhận được: 0x");
  Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
  Serial.print("Giao thức: ");
  Serial.println(IrReceiver.decodedIRData.protocol);

  if (IrReceiver.decodedIRData.decodedRawData == 0x101){
    myServo.write(30);
    delay(500);
    myServo.write(0);
    delay(500);
  }
  else {
    myServo.write(90);
    delay(500);
    myServo.write(0);
    delay(500);
  }

}