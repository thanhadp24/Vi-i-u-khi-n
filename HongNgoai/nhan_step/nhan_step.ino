#include <IRremote.hpp>

#include <Stepper.h>
#define STEPS 2048  // 32 steps * gear reduction ratio 64
#define STEPPER_PIN_1 4
#define STEPPER_PIN_2 5
#define STEPPER_PIN_3 6
#define STEPPER_PIN_4 7

const int IR_RECEIVE_PIN = 2;
int step_number = 0;

void setup() {
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  Serial.begin(9600); // Khởi tạo kết nối Serial
  // pinMode(LED_Pin, OUTPUT); // Thiết lập LED_Pin là ngõ ra
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Bắt đầu bộ nhận IR
}

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
    for (int a = 0; a < 2048/8; a++){   // Buoc quay 45 độ
        OneStep(true);
        delay(2);
      }
  }
  else {
   for (int a = 0; a < 2048/4; a++){   // Buoc quay 90 độ
      OneStep(false);
      delay(2);
  }
  delay(400);
  }
}

void OneStep(bool dir){
  if(dir){
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }
  }
  else{
    switch(step_number){
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
    }
  }
  step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}