#include <IRremote.hpp>

const int IR_SEND_PIN = 3;
const int FEEDBACK_LED_PIN = 12; // Optional: Pin for LED feedback
#include <DHT.h>
const int dht11_pin = 8;
const int dht_model = DHT11;
float humidity;
float temperature;
DHT dht11(dht11_pin, dht_model);

bool turnOn = true;
const unsigned long LED_ON_CODE = 0x1; // Define a constant for clarity
const unsigned long LED_OFF_CODE = 0x0;

void setup() {
Serial.begin(9600); // Initialize serial communication
IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK, FEEDBACK_LED_PIN); // Start the IR sender with feedback
dht11.begin();
}

// Pin for sending IR signals

void loop(){
humidity = dht11.readHumidity();
  temperature = dht11.readTemperature();
  
if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Không có giá trị!");
  } else {
    Serial.print("Độ ẩm: ");
    Serial.print(humidity);

    Serial.print("%; Nhiêt độ: ");
    Serial.print(temperature);
    Serial.println("\u2103");
  }


// Transmit the IR signal
if (temperature < 28) {
IrSender.sendRC5(LED_ON_CODE, 8); // Send RC5 signal with value 0x1
Serial.println("Sent RC5 ON Code");
Serial.println(LED_ON_CODE);
} else {
IrSender. sendRC5(LED_OFF_CODE, 8); // Send RC5 signal with value 0x0
Serial.println("Sent RC5 OFF Code");
Serial.println(LED_OFF_CODE);
}



delay(2000); // 1-second delay to separate send cycles
}