#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>
#define DHTPIN D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// Cấu hình WiFi
const char* ssid = "Quang Minh";       // Tên WiFi
const char* password = "khongcanbiet"; // Mật khẩu WiFi

// Cấu hình chân điều khiển động cơ DC
#define IN1 D1   // Chân điều khiển hướng 1
#define IN2 D2   // Chân điều khiển hướng 2

// Cấu hình cảm biến

// Tạo máy chủ web
ESP8266WebServer server(80);

int dirStatus = 0;  // Trạng thái điều hướng: 0 = Dừng, 1 = CCW, 2 = CW

String readDHTValues() {
  float temperature = dht.readTemperature(); // Đọc nhiệt độ (°C)
  float humidity = dht.readHumidity();       // Đọc độ ẩm (%)

  // Kiểm tra lỗi
  if (isnan(temperature) || isnan(humidity)) {
    return "Lỗi khi đọc cảm biến!";
  }

  return "Nhiệt độ: " + String(temperature) + "°C | Độ ẩm: " + String(humidity) + "%";
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
  function updateDHT11Status() {\
  var xhttp = new XMLHttpRequest();\
  xhttp.onreadystatechange = function() {\
    if (this.readyState == 4 && this.status == 200) {\
      document.getElementById('dht11Status').innerHTML = 'Nhiệt độ & Độ ẩm: ' + this.responseText;\
    }\
  };\
  xhttp.open('GET', '/dht11', true);\
  xhttp.send();\
}\
setInterval(updateDHT11Status, 2000); // Cập nhật dữ liệu mỗi 2 giây\
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
  HTML += "<div class='status' id='dht11Status'>Nhiệt độ & Độ ẩm: Đang tải...</div>";
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

void handleDHT11() {
  String dhtValues = readDHTValues();
  server.send(200, "text/plain", dhtValues);
}

void setup() {
  dht.begin();  // Khởi động cảm biến DHT11
  Serial.begin(9600); // Khởi tạo Serial
  Serial.println("DC Motor & Sensor Control");

  // Cấu hình chân điều khiển động cơ
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  // Cấu hình chân cảm biến
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
  server.on("/dht11", HTTP_GET, handleDHT11); // Endpoint DHT11
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Xử lý client HTTP
  MDNS.update();         // Cập nhật MDNS
}
