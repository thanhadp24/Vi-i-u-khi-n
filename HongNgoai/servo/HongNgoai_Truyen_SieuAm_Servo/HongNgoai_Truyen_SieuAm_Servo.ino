#include <IRremote.hpp>

const int IR_SEND_PIN = 3;
const int FEEDBACK_LED_PIN = 12; // Optional: Pin for LED feedback
int value;

const int trigger_pin = 4;
const int pw_pin = 5;// echo 
const int trigger_delay = 10;
long duration;
float cm;

bool turnOn = true;
const unsigned long LED_ON_CODE = 0x1; // Define a constant for clarity
const unsigned long LED_OFF_CODE = 0x0;

void setup() {
Serial.begin(9600); // Initialize serial communication
IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK, FEEDBACK_LED_PIN); // Start the IR sender with feedback
pinMode(trigger_pin, OUTPUT);
  pinMode(pw_pin, INPUT);
}

// Pin for sending IR signals

void loop(){
digitalWrite(trigger_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(trigger_delay);
  digitalWrite(trigger_pin, LOW);
  duration = pulseIn(pw_pin, HIGH);
  cm = duration * 0.017;
  Serial.print(cm);
  Serial.println(" cm");
  


// Transmit the IR signal
if (cm <= 12.5) {
IrSender.sendRC5(LED_ON_CODE, 8); // Send RC5 signal with value 0x1
Serial.println("Sent RC5 ON Code");
Serial.println(LED_ON_CODE);
} else {
IrSender. sendRC5(LED_OFF_CODE, 8); // Send RC5 signal with value 0x0
Serial.println("Sent RC5 OFF Code");
Serial.println(LED_OFF_CODE);
}



delay(1000); // 1-second delay to separate send cycles
}