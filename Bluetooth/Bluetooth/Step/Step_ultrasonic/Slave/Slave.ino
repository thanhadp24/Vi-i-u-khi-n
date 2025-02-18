
// Code thủ công

/*
// Slave
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11);
char command;
// Step
#include <Stepper.h>
#define STEPS 2048  // 32 steps * gear reduction ratio 64
#define STEPPER_PIN_1 4
#define STEPPER_PIN_2 5
#define STEPPER_PIN_3 6
#define STEPPER_PIN_4 7

int step_number = 0;
void setup() {
  Serial.begin(9600);
  BTSerial.begin(38400);

  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
}

void loop() {
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Received command: ");
    Serial.println(command);
    if (command == '0') {
      for (int a = 0; a < 2048/4; a++){   
        OneStep(true);
        delay(2);
      }
    } else if (command == '1') {
        for (int a = 0; a < 2048/4; a++){   
            OneStep(false);
            delay(2);
        }
    }
  }
  delay(500);
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
*/

// #include <SoftwareSerial.h>

// SoftwareSerial BTSerial(10, 11);  // RX | TX

// void setup() {
//   pinMode(9, OUTPUT);
//   digitalWrite(9, HIGH);
//   Serial.begin(9600);
//   Serial.println("Enter AT commands:");
//   BTSerial.begin(38400);  // HC-05 default speed in AT command
// }
// void loop() {
//   // Keep reading from HC-05 and send to Arduino Serial Monitor
//   if (BTSerial.available()) {
//     Serial.write(BTSerial.read());
//   }
//   if (Serial.available()) {
//     BTSerial.write(Serial.read());
//   }
//   // Keep reading from Arduino Serial Monitor and send to HC-05
// }






// Code dùng thư viện Stepper.h
#include <SoftwareSerial.h>
#include <Stepper.h>

// Định nghĩa số bước trên một vòng quay
#define STEPS 2048

// Khai báo chân cho động cơ bước
#define STEPPER_PIN_1 4
#define STEPPER_PIN_2 5
#define STEPPER_PIN_3 6
#define STEPPER_PIN_4 7

// Tạo đối tượng Stepper
Stepper myStepper(STEPS, STEPPER_PIN_1, STEPPER_PIN_3, STEPPER_PIN_2, STEPPER_PIN_4);

// Khai báo chân cho giao tiếp Bluetooth
SoftwareSerial BTSerial(10, 11);  // RX | TX

char command;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(38400);

  // Thiết lập tốc độ động cơ (RPM)
  myStepper.setSpeed(15);
}

void loop() {
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Received command: ");
    Serial.println(command);

    // Điều khiển động cơ bước theo lệnh nhận được
    if (command == '0') {
      int angle = 90;
      int s = map(angle,0,360,0,2048);   
      myStepper.step(s);  // Quay 90° theo chiều kim đồng hồ
    } 
    else if (command == '1') {
      int angle = 90;
      int s = map(angle,0,360,0,2048);
      s = -s;   
      myStepper.step(s); // Quay 90° ngược chiều kim đồng hồ
    }
  }
  delay(500);
}




























