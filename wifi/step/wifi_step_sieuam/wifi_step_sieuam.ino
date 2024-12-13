#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

const char *ssid = "@@@@@@@";
const char *password = "aaaaaaaaa";
//lỏ lắm
// Ultrasonic sensor pins
const int TRIG_PIN = D5;
const int ECHO_PIN = D7;

float distance;
int angle = 0;
const int stepPerRevolution = 2048;
Stepper myStepper = Stepper(2048, D3, D1, D2, D0);

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
  <html>
  <head>
    <title>STEPPER MOTOR & ULTRASONIC</title>
  </head>
  <body>
    <div>
      <p style="font-size: 25px"><b>Khoang cach:</b> <span id="sensor_value"></span> cm</p>
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
      .then(data => {
        document.getElementById("sensor_value").textContent = data;
      });
  }

  document.getElementById('set_angle-btn').addEventListener('click', setAngle);
  setInterval(readSensorValue, 1000);
)rawliteral";

long readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  long res = duration * 0.034 / 2; // Convert to cm
  if(res > 10000){
    res = 999;
  }
  return res;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  myStepper.setSpeed(18);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
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

// Keep all stepper motor related code unchanged
float current_step_position = 0;
float target_step_position = 0;
void updateStepper() {
  target_step_position = map(angle, 0, 360, 0, 2048);
  if (current_step_position < target_step_position) {
    current_step_position += 1;
    myStepper.step(1);
  } else if (current_step_position > target_step_position) {
    current_step_position -= 1;
    myStepper.step(-1);
  }
}

int step_count_delay = 0;
int step_max_delay = 50;
float steps;

void loop() {
  

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

  distance = readDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void handleRoot(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", index_html);
}

void handleScript(AsyncWebServerRequest *request) {
  request->send_P(200, "text/javascript", script_js);
}

void handleSendSensorValue(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", String(readDistance()));
}

void handleSetAngle(AsyncWebServerRequest *request) {
  if (request->hasParam("angle"))
    angle = request->getParam("angle")->value().toInt();
  request->send(200, "text/plain", "set success");
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "404 ERROR!");
}