// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10,11);
// sensor
const int linePin = 8;
int value;
//
void setup() {
  BTSerial.begin(38400);
  Serial.begin(9600);
}

void loop() {
  value = digitalRead(linePin);
  Serial.println(value);
  if (value == LOW){ // Trắng(phản xạ ánh sáng) cho giá trị LOW (0)
    BTSerial.write('0');
  } else {          // Đen(không phản xạ) cho giá trị HIGH (1)
    BTSerial.write('1');
  }
  delay(1000);
}