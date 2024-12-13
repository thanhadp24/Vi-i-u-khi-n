// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10,11);
// sensor
const int linePin = 2;
int value;
//
void setup() {
  BTSerial.begin(38400);
  Serial.begin(9600);
}

void loop() {
  value = digitalRead(linePin);
  Serial.println(value);
  if (value == 0) { //  có vật cản line trang
    BTSerial.write('0');
  } else {
    BTSerial.write('1');
  }
  delay(1500);
}