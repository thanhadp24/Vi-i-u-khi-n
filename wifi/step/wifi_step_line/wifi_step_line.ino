#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

const char *ssid = "@@@@@@@";
const char *password = "aaaaaaaaa";
//done 
int sensor = 0;
const int SENSOR_PIN = D5;

int angle = 0;
const int stepPerRevolution = 2048;
Stepper myStepper = Stepper(2048, D3, D1, D2, D0); // INT1 INT3 INT2 INT4

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
  <html>
  <head>
    <title>STEPPER MOTOR</title>
  </head>
  <body>
    <div>
      <p style="font-size: 25px"><b>Phat hien vat can bang do line:</b> <span id="sensor_value"></span></p>
    </div>
    <div style="font-size: 25px">
        <label for=""><b>Rotate for stepper motor:</b> </label>
        <br>
        <input type="text" id="angle" value="">
        <br>
        <button id="set_angle-btn">Set</button>
    </div>
    <script src="script.js"></script>
  </body>
</html>
)rawliteral";

const char script_js[] PROGMEM = R"rawliteral(
  const setAngle = () => {
    const angle = document.getElementById('angle').value;
    fetch("/motor?angle=" + angle);
    console.log(angle);
  }

  const readSensorValue = () => {  
    fetch("/sensor")
      .then(response => response.text())
      .then(data =>
        document.getElementById("sensor_value").textContent = (data == '1') ? 'Toi' : 'Sang');
  }

  document.getElementById('set_angle-btn').addEventListener('click', setAngle);
  setInterval(readSensorValue, 200);
)rawliteral";

void setup() {
  // put your setup code here, to run once:
  pinMode(SENSOR_PIN, INPUT);
  myStepper.setSpeed(18);
  Serial.begin(115200);
  // WiFi.softAP(ssid, password);
  // Serial.print("IP: ");
  // Serial.println(WiFi.softAPIP());
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/script.js", handleScript);
  server.on("/sensor", handleSendSensorValue);
  server.on("/motor", handleSetAngle);
  server.onNotFound(handleNotFound);
  server.begin();
}

float current_step_position = 0;
float target_step_position = 0;

void updateStepper() {
  // Tính toán số bước mục tiêu từ góc độ
  target_step_position = map(angle, 0, 360, 0, 2048);

  // Chỉ thực hiện bước nhỏ để tiến gần đến vị trí mục tiêu
  if (current_step_position < target_step_position) {
    current_step_position += 1; // Tăng dần bước
    myStepper.step(1);          // Thực hiện 1 bước tiến
  } else if (current_step_position > target_step_position) {
    current_step_position -= 1; // Giảm dần bước
    myStepper.step(-1);         // Thực hiện 1 bước lùi
  }
}

int step_count_delay = 0;
int step_max_delay = 50;
float steps;
void loop() {
  sensor = digitalRead(SENSOR_PIN);
  if(isnan(sensor))
  {}
  else{
    // Serial.print("Gia tri: ");
    // Serial.println(sensor);ze
  }

  if (step_count_delay == 0) {
    steps = map(angle, 0, 360, 0, 2048);
    Serial.print("Angle: ");
    Serial.println(angle);
    Serial.print("Steps: ");
    Serial.println(steps);
    myStepper.step(steps);
  }
  step_count_delay++;
  if(step_count_delay > step_max_delay) step_count_delay = 0;
  delay(10);
 
}

void handleRoot(AsyncWebServerRequest *request) {
  request->send(200, "text/html", index_html);
}

void handleScript(AsyncWebServerRequest *request) {
  request->send(200, "text/javascript", script_js);
}

void handleSendSensorValue(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", String(sensor));
}

void handleSetAngle(AsyncWebServerRequest *request) {
  if (request->hasParam("angle"))
    angle = request->getParam("angle")->value().toInt();
  request->send(200, "text/plain", "set success");
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "404 ERROR!");
}