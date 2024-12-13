const int line_d_pin = 7;
const int line_a_pin = A0;

const int stepsPerRevolution = 2048;  // Số bước cho 1 vòng quay
const int stepAngle1 = 90;            // Góc quay 90 độ
const int stepAngle2 = 180;           // Góc quay 180 độ

const int stepCount1 = map(stepAngle1, 0, 360, 0, stepsPerRevolution);
const int stepCount2 = map(stepAngle2, 0, 360, 0, stepsPerRevolution);

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  myStepper.setSpeed(15); 
  Serial.begin(9600);
  pinMode(line_d_pin, INPUT);
}

void loop() {
  int d_value = digitalRead(line_d_pin);
  int a_value = analogRead(line_a_pin);

  Serial.println(d_value);
  Serial.println(a_value);
  Serial.println();

  if (a_value < 877){
    myStepper.step(stepCount2);   // Quay thuận 180 độ
    delay(500);
    return;
  }
  else {
    myStepper.step(-stepCount2);  // Quay ngược 180 độ
    delay(500);
    return;
  }
  delay(100);
}