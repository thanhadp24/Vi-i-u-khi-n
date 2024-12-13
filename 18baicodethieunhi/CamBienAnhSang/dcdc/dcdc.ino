int in1 = 7;
int in2 = 6;
int e = 9;
int ldrPin = A0; // Chân cảm biến ánh sáng
int ldrValue; // Biến lưu giá trị ánh sáng

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(e, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  ldrValue = analogRead(ldrPin); // Đọc giá trị từ cảm biến ánh sáng
  Serial.println(ldrValue); // In giá trị ánh sáng để debug

  if (ldrValue > 877){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(e, 255);
    delay(500);
  }

  else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(e, 255);
    delay(500);
  }
}