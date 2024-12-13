// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10,11);
// sensor
int vrx, vry;
const int SW_pin = 5;
//chinh sua lai logic
void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  BTSerial.begin(9600);
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
    Serial.println("South");
    // Gạt xuống
    BTSerial.write('0');
  } else if (vrx >= 800) {
    Serial.println("North");
    // Gạt lên
    BTSerial.write('1');
  }
  if (vry <= 200) {
    Serial.println("West");
    // Gạt trái: 
    BTSerial.write('2');
  } else if (vry >= 800) {
    Serial.println("East");
    // Gạt phải: 
    BTSerial.write('3');
  }
  
  if (SW_value == LOW) {
    Serial.println("CENTER");
    BTSerial.write('3');

  }

  delay(1500);
}
