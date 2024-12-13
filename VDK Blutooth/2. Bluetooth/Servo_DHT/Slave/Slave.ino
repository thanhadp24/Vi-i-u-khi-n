#include <SoftwareSerial.h>
#include <Servo.h>
Servo myServo;
SoftwareSerial BTSerial(10, 11);
//delay 500 thoi
void setup() {
  myServo.attach(9,500,2500);
  Serial.begin(38400);
  Serial.println("Enter your commands");

  BTSerial.begin(38400);
}

void loop() {
  if (BTSerial.available()) {
    char receivedChar = BTSerial.read();
    Serial.write(receivedChar);

    switch (receivedChar) {
      case '0':
        myServo.write(120);
        delay(1000);
        myServo.write(0);
        delay(1000);
        break;
      case '1':
        myServo.write(30);
        delay(1000);
        myServo.write(0);
        delay(1000);
        break;
      // Add more cases if needed
      default:
        // Handle other cases if neede
        myServo.write(0);
        delay(15);
        break;
    }
    Serial.println();
    // delay(100);
  }
}
  