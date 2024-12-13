// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10,11);
// sensor
const int linePin = 2; // chân số còn ở cảm bién là chân d0
int value;
//
void setup() {  
  BTSerial.begin(38400);
  Serial.begin(9600);
}

void loop() {
  value = digitalRead(linePin);
  Serial.println(value);
  if (value == 0) { // có vật cản
    BTSerial.write('0');
  } else {
    BTSerial.write('1');
  }
  delay(2000);
}