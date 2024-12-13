int in1 = 7;
int in2 = 6;
int e = 9;
const int trigPin = 5; // 
const int echoPin = 3; // 
long duration; // Định nghĩa biến thời gian
int distance; // Định nghĩa biến khoảng cách

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(e, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance Measured = "); 
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 10){
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
