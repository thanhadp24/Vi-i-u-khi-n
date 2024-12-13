const int line_d_pin = 11;
const int line_a_pin = A0;
int in1 = 7;
int in2 = 6;
int e = 9;

void setup() {
  Serial.begin(9600);
  pinMode(line_d_pin, INPUT);
}

void loop() {
  int d_value = digitalRead(line_d_pin);
  int a_value = analogRead(line_a_pin);

  Serial.println(d_value);
  Serial.println(a_value);
  Serial.println();

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