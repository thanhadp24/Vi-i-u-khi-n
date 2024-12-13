#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Stepper.h>

// Thông tin WiFi
const char* ssid = "Quang Minh";
const char* password = "khongcanbiet";

// Cấu hình động cơ bước
const int stepsPerRevolution = 2048; // Số bước cho mỗi vòng quay của động cơ 28BYJ-48
Stepper myStepper(stepsPerRevolution, D1, D3, D2, D4);
int trigPin = D5; // Chân Trig
int echoPin = D6; // Chân Echo

// Tạo server
ESP8266WebServer server(80);

long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Đổi thời gian sang khoảng cách (cm)
}

// Giao diện HTML
void handleRoot() {
  String HTML = "<!DOCTYPE html>\
  <html lang='vi'>\
  <head>\
  <meta charset='UTF-8'>\
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
  <title>Điều Khiển Động Cơ & Cảm Biến</title>\
  <style>\
  body { font-family: Arial, sans-serif; text-align: center; background-color: #f0f8ff; margin: 0; padding: 0; }\
  h1 { color: #333; margin-top: 20px; }\
  .status { font-size: 20px; color: #007bff; margin-bottom: 20px; }\
  .input { margin: 20px 0; font-size: 18px; padding: 10px; width: 80px; text-align: center; }\
  .btn { display: inline-block; padding: 10px 20px; font-size: 18px; color: white; background-color: #007bff; border: none; border-radius: 5px; cursor: pointer; }\
  .btn:hover { background-color: #0056b3; }\
  </style>\
  <script>\
  function rotateMotor(direction) {\
    var degrees = document.getElementById('degreesInput').value;\
    var xhttp = new XMLHttpRequest();\
    xhttp.open('GET', '/motor?direction=' + direction + '&degrees=' + degrees, true);\
    xhttp.send();\
  }\
  function updateDistance() {\
    var xhttp = new XMLHttpRequest();\
    xhttp.onreadystatechange = function() {\
      if (this.readyState == 4 && this.status == 200) {\
        document.getElementById('distanceValue').innerHTML = 'Khoảng cách: ' + this.responseText + ' cm';\
      }\
    };\
    xhttp.open('GET', '/distance', true);\
    xhttp.send();\
  }\
  setInterval(updateDistance, 1000); // Cập nhật khoảng cách mỗi giây\
  </script>\
  </head>\
  <body>\
  <h1>Điều Khiển Động Cơ & Cảm Biến Siêu Âm</h1>\
  <input type='number' id='degreesInput' class='input' min='0' max='360' placeholder='Nhập số độ quay'>\
  <button class='btn' onclick='rotateMotor(\"cw\")'>Quay Cùng Chiều Kim Đồng Hồ</button>\
  <button class='btn' onclick='rotateMotor(\"ccw\")'>Quay Ngược Chiều Kim Đồng Hồ</button>\
  <div class='status' id='distanceValue'>Khoảng cách: Đang tải...</div>\
  </body>\
  </html>";

  server.send(200, "text/html", HTML);
}

// Điều khiển động cơ bước
void motorControl() {
  String direction = server.arg("direction");
  String degreesArg = server.arg("degrees");
  int degrees = degreesArg.toInt();
  int steps = (stepsPerRevolution * degrees) / 360;

  if (direction == "cw") {
    myStepper.step(steps);
    server.send(200, "text/plain", "Động cơ quay cùng chiều kim đồng hồ " + String(degrees) + " độ");
  } else if (direction == "ccw") {
    myStepper.step(-steps);
    server.send(200, "text/plain", "Động cơ quay ngược chiều kim đồng hồ " + String(degrees) + " độ");
  } else {
    server.send(400, "text/plain", "Hướng quay không hợp lệ");
  }
}

void handleDistanceSensor() {
  long distance = measureDistance();
  server.send(200, "text/plain", String(distance));
}

void handleNotFound() {
  server.send(404, "text/plain", "Không tìm thấy trang yêu cầu");
}

void setup() {
  myStepper.setSpeed(15);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("Động Cơ Bước & Cảm Biến");

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
  if (MDNS.begin("motorESP8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);                  // Giao diện chính
  server.on("/motor", HTTP_GET, motorControl); // Điều khiển động cơ bước
  server.on("/distance", HTTP_GET, handleDistanceSensor); //Giá trị cảm biến siêu âm
  server.onNotFound(handleNotFound);          // Xử lý trang lỗi
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  MDNS.update();
}
