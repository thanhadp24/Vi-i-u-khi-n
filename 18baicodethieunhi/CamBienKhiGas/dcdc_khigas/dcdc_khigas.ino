int gasPin = A0;
int val;
int in1 = 7;
int in2 = 6;
int e = 9;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(e, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  val = analogRead(gasPin);
  Serial.println(val);

  if (val > 877){
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
  delay(500);
}