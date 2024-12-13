#include <IRremote.hpp>

const int IR_SEND_PIN = 3;
const int FEEDBACK_LED_PIN = 12; // Optional: Pin for LED feedback
// sensor
int vrx, vry;
int SW_pin = 5; // Nút nhấn trên joystick

bool turnOn = true;
const unsigned long LED_ON_CODE = 0x1; // Define a constant for clarity
const unsigned long LED_OFF_CODE = 0x0;
const unsigned long CUSTOM2 = 0x2;
const unsigned long CUSTOM3 = 0x3;

void setup() {
Serial.begin(9600); // Initialize serial communication
IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK, FEEDBACK_LED_PIN); // Start the IR sender with feedback
}

// Pin for sending IR signalsZ

void loop(){
  vrx = analogRead(A0);
  vry = analogRead(A1);
  Serial.print("x: ");
  Serial.print(vrx);
  Serial.print(", y: ");
  Serial.print(vry);

  int SW_value = digitalRead(SW_pin);
  Serial.print(", z: ");
  Serial.print(SW_value); 
  Serial.println();

  if (vrx <= 200){
    Serial.print("South\n");
    // Gat xuong
    IrSender.sendRC5(LED_ON_CODE, 8); // Send RC5 signal with value 0x1
    Serial.println("Sent RC5 ON Code");
    Serial.println(LED_ON_CODE);
  }
  else if (vrx >= 800){
    Serial.print("North\n");
    // Gạt lên
    IrSender. sendRC5(LED_OFF_CODE, 8); // Send RC5 signal with value 0x0
    Serial.println("Sent RC5 OFF Code");
    Serial.println(LED_OFF_CODE);
  }
  else if (vry <= 300){
    Serial.print("West\n");
    // Gạt trái: 
    IrSender.sendRC5(CUSTOM3, 8); // Send RC5 signal with value 0x1
    Serial.println("Sent RC5 ON Code");
    Serial.println(CUSTOM3);
  }
  else if (vry >= 800){
    Serial.print("East\n");
    // Gạt phải: 
    IrSender. sendRC5(CUSTOM2, 8); // Send RC5 signal with value 0x0
    Serial.println("Sent RC5 OFF Code");
    Serial.println(CUSTOM2);
   
  }else if(SW_pin == 0){
    Serial.print("center\n");
    
    IrSender. sendRC5(CUSTOM2, 8); // Send RC5 signal with value 0x0
    Serial.println("Sent RC5 OFF Code");
    Serial.println(CUSTOM2);
  }


delay(1000); // 1-second delay to separate send cycles
}