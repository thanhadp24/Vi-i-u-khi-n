#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Cấu hình WiFi
const char* ssid = "@@@@@@@";       // Tên WiFi
const char* password = "aaaaaaaaa"; // Mật khẩu WiFi

// Cấu hình chân điều khiển động cơ DC
#define IN1 D6   // Chân điều khiển hướng 1
#define IN2 D5   // Chân điều khiển hướng 2

// Cấu hình cảm biến
int trigPin = D1; // Chân Trig
int echoPin = D2; // Chân Echo
// Tạo máy chủ web
ESP8266WebServer server(80);

int dirStatus = 0;  // Trạng thái điều hướng: 0 = Dừng, 1 = CCW, 2 = CW

long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Đổi thời gian sang khoảng cách (cm)
}

// Hàm điều khiển giao diện web
void handleRoot() {
  String HTML = "<!DOCTYPE html>\
  <html>\
  <head>\
  <meta charset=\"utf-8\">\
  <title>Điều khiển động cơ & Cảm biến</title>\
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  <style>\
  html,body{width:100%;height:100%;margin:0;font-family:Arial,sans-serif;text-align:center}\
  .btn{margin:30px;padding:15px;font-size:30px;text-align:center;background-color:#add8e6;border:none;cursor:pointer}\
  .btn:hover{background-color:#90ee90}\
  </style>\
  <script>\
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
  <h1>Điều khiển động cơ & Cảm biến</h1>";

  if (dirStatus == 2) {
    HTML += "<h2 style='color: green;'>Động cơ đang chạy thuận (CW)</h2>";
  } else if (dirStatus == 1) {
    HTML += "<h2 style='color: green;'>Động cơ đang chạy ngược (CCW)</h2>";
  } else {
    HTML += "<h2 style='color: red;'>Động cơ đang dừng</h2>";
  }

  HTML += "<a class='btn' href='/motor?direction=ccw'>Chạy ngược</a><br><br><br>";
  HTML += "<a class='btn' href='/motor?direction=cw'>Chạy thuận</a><br><br><br>";
  HTML += "<a class='btn' href='/motor?direction=off'>Dừng động cơ</a>";
  HTML += "<h2>Dữ liệu cảm biến</h2>";
  HTML += "<div class='status' id='distanceValue'>Khoảng cách: Đang tải...</div>";
  HTML += "</body></html>";


  server.send(200, "text/html; charset=utf-8", HTML);
}

// Hàm xử lý khi không tìm thấy trang
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

// Hàm điều khiển động cơ DC
void motorControl() {
  String direction = server.arg("direction");
  if (direction == "ccw") {
    dirStatus = 1; // Chạy CCW
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else if (direction == "cw") {
    dirStatus = 2; // Chạy CW
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (direction == "off") {
    dirStatus = 0; // Dừng động cơ
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }
  handleRoot(); // Cập nhật giao diện web
}

void handleDistanceSensor() {
  long distance = measureDistance();
  server.send(200, "text/plain", String(distance));
}

void setup() {
  Serial.begin(9600); // Khởi tạo Serial
  Serial.println("DC Motor & Sensor Control");

  // Cấu hình chân điều khiển động cơ
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  // Cấu hình chân cảm biến
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // pinMode(LDRPinDigital, INPUT);

  // Đặt trạng thái ban đầu cho động cơ
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Cấu hình WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266motor")) {
    Serial.println("MDNS responder started");
    Serial.println("Access via http://esp8266motor");
  }

  server.on("/", handleRoot);
  server.on("/motor", HTTP_GET, motorControl);
  server.on("/distance", HTTP_GET, handleDistanceSensor); //Giá trị cảm biến siêu âm
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Xử lý client HTTP
  MDNS.update();         // Cập nhật MDNS
}
