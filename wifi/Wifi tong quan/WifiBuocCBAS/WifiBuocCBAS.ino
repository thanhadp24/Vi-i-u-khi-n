#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Stepper.h>

// Kết nối Wi-Fi
const char* ssid = "TUHOC_KHU_A";
const char* password = "";

// Cấu hình động cơ bước
const int stepsPerRevolution = 2048; // Số bước cho mỗi vòng quay của động cơ 28BYJ-48
Stepper myStepper(stepsPerRevolution, D1, D3, D2, D4);

// Cấu hình cảm biến
const int LDRPinAnalog = A0;   // Chân analog (đọc cường độ ánh sáng)
const int LDRPinDigital = D5;  // Chân digital (phát hiện vạch kẻ)

// Tạo máy chủ web
ESP8266WebServer server(80);

int dirStatus = 3; // Trạng thái điều hướng: 3 = Dừng, 1 = CCW, 2 = CW
int lightValueAnalog = 0;  // Giá trị đọc từ cảm biến analog
int lightValueDigital = 0; // Giá trị đọc từ cảm biến digital

// Hàm điều khiển giao diện web
void handleRoot() {
  String HTML = "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Motor & Sensor Control</title>\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  <style>\
  html,body{width:100%;height:100%;margin:0}\
  .btn{margin:5px;padding:15px;font-size:30px;text-align:center;background-color:#add8e6;border:none;cursor:pointer}\
  .btn:hover{background-color:#90ee90}\
  </style>\
  <script>\
  function updateSensorData() {\
    var xhr = new XMLHttpRequest();\
    xhr.onreadystatechange = function() {\
      if (xhr.readyState == 4 && xhr.status == 200) {\
        var data = JSON.parse(xhr.responseText);\
        document.getElementById('ldrValueAnalog').innerHTML = data.analog;\
        document.getElementById('ldrValueDigital').innerHTML = data.digital ? 'Line Detected' : 'No Line';\
      }\
    };\
    xhr.open('GET', '/readSensor', true);\
    xhr.send();\
  }\
  setInterval(updateSensorData, 1000);\
  </script>\
  </head>\
  <body>\
  <h1>Motor & Sensor Control</h1>";

  if (dirStatus == 2) {
    HTML += "<h2 style='color: green;'>Motor Running in CW</h2>";
  } else if (dirStatus == 1) {
    HTML += "<h2 style='color: green;'>Motor Running in CCW</h2>";
  } else {
    HTML += "<h2 style='color: red;'>Motor OFF</h2>";
  }

  HTML += "<a class='btn' href='/motor?direction=ccw'>Run CCW</a><br>";
  HTML += "<a class='btn' href='/motor?direction=cw'>Run CW</a><br>";
  HTML += "<a class='btn' href='/motor?direction=off'>Stop Motor</a>";

  HTML += "<h2>Sensor Data</h2>";
  HTML += "<p>Analog Light Level: <span id='ldrValueAnalog'>Loading...</span></p>";
  HTML += "<p>Digital Line Detection: <span id='ldrValueDigital'>Loading...</span></p>";

  HTML += "</body></html>";

  server.send(200, "text/html", HTML);
}

// Hàm xử lý cảm biến
void handleSensorRead() {
  lightValueAnalog = analogRead(LDRPinAnalog);    // Đọc giá trị từ cảm biến analog
  lightValueDigital = digitalRead(LDRPinDigital); // Đọc giá trị từ cảm biến digital

  // Tạo chuỗi JSON trả về giá trị cảm biến
  String jsonData = "{\"analog\": " + String(lightValueAnalog) +
                    ", \"digital\": " + String(lightValueDigital) + "}";
  
  server.send(200, "application/json", jsonData);  // Gửi dữ liệu về client dưới dạng JSON
}

// Hàm điều khiển động cơ bước
void motorControl() {
  String direction = server.arg("direction");
  if (direction == "ccw") {
    dirStatus = 1; // Chạy CCW
  } else if (direction == "cw") {
    dirStatus = 2; // Chạy CW
  } else if (direction == "off") {
    dirStatus = 3; // Dừng động cơ
  }
  handleRoot(); // Cập nhật giao diện web
}

// Xử lý khi không tìm thấy trang
void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void setup(void) {
  myStepper.setSpeed(15);
  Serial.begin(9600);

  // Cấu hình WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/motor", HTTP_GET, motorControl);
  server.on("/readSensor", HTTP_GET, handleSensorRead);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient(); // Xử lý client HTTP
  MDNS.update();         // Cập nhật MDNS

  if (dirStatus == 1) {  // Chạy CCW
    myStepper.step(-stepsPerRevolution / 100);
  } else if (dirStatus == 2) {  // Chạy CW
    myStepper.step(stepsPerRevolution / 100);
  }
}
