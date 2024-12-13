#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include <thread>

// Thông tin WiFi
const char* ssid = "TUHOC_KHU_A";    // Tên WiFi
const char* password = "";           // Mật khẩu WiFi



// Khởi tạo server và Servo
ESP8266WebServer server(80);        
Servo myServo;                      
int servoPin = D4;                  
int currentPos = 90;                // Vị trí mặc định của Servo
int lightSensorPin = A0;            // Chân cảm biến ánh sáng
bool servoState = false;  


int angle = 0;
unsigned long previousMillis = 0;
const long interval = 1000;


// Hàm đọc giá trị cảm biến ánh sáng
int readLightSensor() {
  return analogRead(lightSensorPin); // Đọc giá trị từ chân analog
}

// Giao diện HTML
void handleRoot() {
  int lightValue = readLightSensor(); // Lấy giá trị cảm biến ánh sáng
  String HTML = "<!DOCTYPE html>\
  <html lang='vi'>\
  <head>\
  <meta charset='UTF-8'>\
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
  <title>Điều khiển Servo & Cảm biến ánh sáng</title>\
  <style>\
  body { font-family: Arial, sans-serif; text-align: center; background-color: #f0f8ff; margin: 0; padding: 0; }\
  h1 { color: #333; margin-top: 20px; }\
  .status { font-size: 20px; color: #007bff; margin-bottom: 20px; }\
  .input { margin: 20px 0; font-size: 18px; padding: 10px; width: 80px; text-align: center; }\
  .btn { display: inline-block; padding: 10px 20px; font-size: 18px; color: white; background-color: #007bff; border: none; border-radius: 5px; cursor: pointer; }\
  .btn:hover { background-color: #0056b3; }\
  </style>\
  <script>\
  function sendAngle() {\
    var angle = document.getElementById('angleInput').value;\
    if (angle >= 0 && angle <= 180) {\
      var xhttp = new XMLHttpRequest();\
      xhttp.onreadystatechange = function() {\
        if (this.readyState == 4 && this.status == 200) {\
          document.getElementById('status').innerHTML = 'Vị trí hiện tại của Servo: ' + angle + '°';\
        }\
      };\
      xhttp.open('GET', '/servo?angle=' + angle, true);\
      xhttp.send();\
    } else {\
      alert('Vui lòng nhập góc từ 0 đến 180.');\
    }\
  }\
  function refreshLight() {\
    var xhttp = new XMLHttpRequest();\
    xhttp.onreadystatechange = function() {\
      if (this.readyState == 4 && this.status == 200) {\
        document.getElementById('lightValue').innerHTML = 'Giá trị cảm biến ánh sáng: ' + this.responseText;\
      }\
    };\
    xhttp.open('GET', '/light', true);\
    xhttp.send();\
  }\
  setInterval(refreshLight, 1000); // Tự động cập nhật giá trị cảm biến mỗi giây\
  </script>\
  </head>\
  <body>\
  <h1>Điều khiển Servo & Cảm biến ánh sáng</h1>\
  <div class='status' id='status'>Vị trí hiện tại của Servo: " + String(currentPos) + "°</div>\
  <input type='number' id='angleInput' class='input' min='0' max='180' placeholder='Nhập góc'>\
  <button class='btn' onclick='sendAngle()'>Đặt góc</button>\
  <div class='status' id='lightValue'>Giá trị cảm biến ánh sáng: " + String(lightValue) + "</div>\
  </body>\
  </html>";

  server.send(200, "text/html", HTML);
}

// Điều khiển Servo qua tham số góc
void handleServoControl() {
  if (server.hasArg("angle")) {
    angle = server.arg("angle").toInt();
    if (angle >= 0 && angle <= 180) {        
      currentPos = angle;             
      Serial.println("Servo moved to " + String(currentPos) + "°");
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Góc không hợp lệ. Vui lòng nhập góc từ 0 đến 180.");
    }
  } else {
    server.send(400, "text/plain", "Yêu cầu không hợp lệ. Cần có tham số 'angle'.");
  }
}

// Trả về giá trị cảm biến ánh sáng
void handleLightSensor() {
  int lightValue = readLightSensor();
  Serial.println(lightValue);
  server.send(200, "text/plain", String(lightValue));
}

void handleNotFound() {
  server.send(404, "text/plain", "Không tìm thấy trang yêu cầu.");
}

void setup() {
  myServo.attach(servoPin, 500, 2500);
  myServo.write(currentPos);        
  Serial.begin(9600);              
  Serial.println("Servo Control & Light Sensor");

  // Kết nối WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Đang kết nối WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối WiFi");
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP());

  // Khởi tạo dịch vụ MDNS
  if (MDNS.begin("servoESP8266")) {
    Serial.println("MDNS responder started");
    Serial.println("Truy cập http://servoESP8266 để điều khiển Servo");
  }

  server.on("/", handleRoot);                 // Giao diện chính
  server.on("/servo", HTTP_GET, handleServoControl); // Điều khiển Servo
  server.on("/light", HTTP_GET, handleLightSensor);  // Giá trị cảm biến ánh sáng
  server.onNotFound(handleNotFound);          // Xử lý trang lỗi
  server.begin();                             
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); 
  MDNS.update();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (servoState) {
      myServo.write(0);       // Quay về góc 0 độ
    } else {
      myServo.write(angle);   // Di chuyển đến góc đã thiết lập
    }
    servoState = !servoState; // Đảo trạng thái servo
  }
}
