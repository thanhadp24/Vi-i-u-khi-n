#include <IRremote.hpp>

const int IR_RECEIVE_PIN = 2;

#define EN12_PIN 10
#define DIR_1A_PIN 8
#define DIR_2A_PIN 9

void dc_motor(int output_1, int output_2, int speed, int delay_ms) {
  digitalWrite(DIR_1A_PIN, output_1);
  digitalWrite(DIR_2A_PIN, output_2);
  analogWrite(EN12_PIN, speed);
  delay(delay_ms);
}

void setup() {

  Serial.begin(9600); // Khởi tạo kết nối Serial
  // pinMode(LED_Pin, OUTPUT); // Thiết lập LED_Pin là ngõ ra
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Bắt đầu bộ nhận IR

  pinMode(EN12_PIN, OUTPUT);
  pinMode(DIR_1A_PIN, OUTPUT);
  pinMode(DIR_2A_PIN, OUTPUT);
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
    dc_motor(LOW, HIGH, 255, 500);
  }
  else if(IrReceiver.decodedIRData.decodedRawData == 0x100) {
   dc_motor(HIGH, LOW, 255, 500);
  } else if(IrReceiver.decodedIRData.decodedRawData == 0x103) {
    dc_motor(HIGH, LOW, 128, 500);
  } else if(IrReceiver.decodedIRData.decodedRawData == 0x102) {
    dc_motor(LOW, HIGH, 128, 500);
  } 
  dc_motor(LOW, LOW, 0, 200);
  //dc_motor(LOW, LOW, 0, 1000);
}