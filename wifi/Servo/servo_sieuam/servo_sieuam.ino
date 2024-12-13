#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
//DONE

// Tạo đối tượng servo
Servo myservo;
int angle = 0;
bool servoState = false;  // Trạng thái servo (bật/tắt)

// Thông tin Wi-Fi
const char* ssid = "@@@@@@@";
const char* password = "aaaaaaaaa";

// Khai báo chân kết nối cảm biến siêu âm và Servo
#define TRIG_PIN D1     // Chân TRIG của cảm biến HC-SR04
#define ECHO_PIN D2     // Chân ECHO của cảm biến HC-SR04
const int Servo_Pin = D3;

// Tạo đối tượng web server
ESP8266WebServer server(80);

// Biến thời gian để điều khiển servo không dùng delay
unsigned long previousMillis = 0;
const long interval = 500;  // Khoảng thời gian (ms) giữa các lần di chuyển servo

// Hàm trả về giao diện HTML
String generateHTML() {
  String html = "<!DOCTYPE html><html><head><title>ESP8266 Ultrasonic and Servo Control</title>";
  html += "<script>";

  // JavaScript để cập nhật giá trị khoảng cách
  html += "function updateSensorData() {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "      var data = JSON.parse(xhr.responseText);";
  html += "      document.getElementById('distance').innerHTML = data.distance;";
  html += "    }";
  html += "  };";
  html += "  xhr.open('GET', '/readSensor', true);";
  html += "  xhr.send();";
  html += "}";

  // JavaScript để điều khiển servo
  html += "function controlServo() {";
  html += "  var angle = document.getElementById('angleInput').value;";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/servoControl?angle=' + angle, true);";
  html += "  xhr.send();";
  html += "}";

  html += "setInterval(updateSensorData, 500);";  // Cập nhật mỗi 1 giây
  html += "</script></head><body>";

  html += "<h1>ESP8266 Ultrasonic and Servo Control</h1>";

  // Hiển thị giá trị khoảng cách
  html += "<p>Distance: <span id='distance'>Loading...</span> cm</p>";

  // Điều khiển servo
  html += "<input type='number' id='angleInput' placeholder='Enter angle (0-180)'>";
  html += "<button onclick='controlServo()'>Move Servo</button>";

  html += "</body></html>";
  return html;
}

// Hàm xử lý yêu cầu đọc giá trị cảm biến siêu âm
void handleSensorRead() {
  long duration, distance;
  
  // Gửi tín hiệu TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Nhận tín hiệu phản hồi từ ECHO
  duration = pulseIn(ECHO_PIN, HIGH);
  
  // Tính khoảng cách (cm)
  distance = duration * 0.0344 / 2;  // Công thức tính khoảng cách
  
  // Trả về dữ liệu khoảng cách dưới dạng JSON
  String json = "{ \"distance\": " + String(distance) + " }";
  server.send(200, "application/json", json);
}

// Hàm xử lý yêu cầu điều khiển servo
void handleServoControl() {
  if (server.hasArg("angle")) {
    int angle_ = server.arg("angle").toInt();  // Đọc góc từ request
    if (angle_ >= 0 && angle_ <= 180) {        // Giới hạn góc từ 0 đến 180 độ
      angle = angle_;
      server.send(200, "text/plain", "Servo moved to " + String(angle) + " degrees");
    } else {
      server.send(400, "text/plain", "Invalid angle. Please enter a value between 0 and 180.");
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Kết nối Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.localIP());
  
  // Khởi động servo
  myservo.attach(Servo_Pin, 500, 2400);
  
  // Đặt chân TRIG và ECHO cho cảm biến siêu âm
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Định nghĩa các route của server
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", generateHTML());
  });
  server.on("/readSensor", HTTP_GET, handleSensorRead);
  server.on("/servoControl", HTTP_GET, handleServoControl);

  server.begin();
}

void loop() {
  server.handleClient();  // Xử lý yêu cầu từ client

  // Kiểm tra thời gian để điều khiển servo
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    if (servoState) {
      myservo.write(0);       // Quay về góc 0 độ
    } else {
      myservo.write(angle);   // Di chuyển đến góc đã thiết lập
    }
    servoState = !servoState; // Đảo trạng thái servo
  }
}
