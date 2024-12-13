// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11);
// sensor
int vrx, vry;
int SW_pin = 5; // Nút nhấn trên joystick
//
void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  BTSerial.begin(38400);
  Serial.begin(9600);
}

void loop() {
  vrx = analogRead(A0);
  vry = analogRead(A1);
  Serial.print("x: ");
  Serial.print(vrx);
  Serial.print(", y: ");
  Serial.print(vry);
  Serial.println();

  // Đọc giá trị từ nút nhấn trên joystick
  int SW_value = digitalRead(SW_pin);

  if (vrx <= 200) {
  Serial.print("South\n");
    // Gạt xuống
    BTSerial.write('0');
  } else if (vrx >= 800) {
  Serial.print("North\n");
    // Gạt lên
    BTSerial.write('1');
  } else if (vry <= 300) {
  Serial.print("West\n");
    // Gạt trái: 
    BTSerial.write('2');
  } else if (vry >= 800) {
  Serial.print("East\n");
    // Gạt phải: 
    BTSerial.write('3');
  }else{
    BTSerial.write('3');
  }

  
  if (SW_value == LOW) {
    Serial.println("CENTER");
    // Gạt phải: Chậm dần
    BTSerial.write('4');
  }

  delay(1000);
}
