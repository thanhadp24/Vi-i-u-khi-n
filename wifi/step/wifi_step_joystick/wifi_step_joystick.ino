#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

const char *ssid = "@@@@@@@";
const char *password = "aaaaaaaaa";
//done
// Joystick pins
const int JOY_X_PIN = A0;    // X axis
const int JOY_BTN_PIN = D5;  // Button

int angle = 0;
const int stepPerRevolution = 2048;
Stepper myStepper = Stepper(2048, D3, D1, D2, D0);

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
  <html>
  <head>
    <title>STEPPER MOTOR & JOYSTICK</title>
    <style>
      body { font-family: Arial; text-align: center; }
      .container { max-width: 600px; margin: 0 auto; padding: 20px; }
      .sensor-data { font-size: 25px; margin: 20px 0; }
    </style>
  </head>
  <body>
    <div class="container">
      <div class="sensor-data">
        <p><b>Joystick X:</b> <span id="joy_x">--</span></p>
        <p><b>Button:</b> <span id="btn_state">--</span></p>
      </div>
      <div style="font-size: 25px">
        <label><b>Rotate for stepper motor:</b></label>
        <br>
        <input type="text" id="angle" value="">
        <br>
        <button id="set_angle-btn">Set</button>
      </div>
    </div>
    <script src="script.js"></script>
  </body>
</html>
)rawliteral";

// Update the JavaScript section in script_js:
const char script_js[] PROGMEM = R"rawliteral(
  const setAngle = () => {
    const angle = document.getElementById('angle').value;
    fetch("/motor?angle=" + angle);
    console.log(angle);
  }

  const readSensorValues = () => {  
    fetch("/sensor")
      .then(response => response.json())
      .then(data => {
        document.getElementById("joy_x").textContent = data.x;
        document.getElementById("btn_state").textContent = data.button;
      });
  }

  document.getElementById('set_angle-btn').addEventListener('click', setAngle);
  setInterval(readSensorValues, 1000); // Increased polling rate for better button response
)rawliteral";

void setup() {
  Serial.begin(115200);
  pinMode(JOY_BTN_PIN, INPUT_PULLUP);
  myStepper.setSpeed(18);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
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
int step_count_delay = 0;
int step_max_delay = 50;
float steps;

void loop() {
  int joyX = analogRead(JOY_X_PIN);
  bool buttonPressed = !digitalRead(JOY_BTN_PIN);  // Active LOW

  Serial.print("X: ");
  Serial.print(joyX);
  Serial.print(" Button: ");
  Serial.println(buttonPressed);

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
  request->send_P(200, "text/html", index_html);
}

void handleScript(AsyncWebServerRequest *request) {
  request->send_P(200, "text/javascript", script_js);
}

// Modify handleSendSensorValue function:
void handleSendSensorValue(AsyncWebServerRequest *request) {
  int buttonState = digitalRead(JOY_BTN_PIN); // Read button state (LOW when pressed)
  String buttonText = buttonState == LOW ? "PRESSED" : "NOT PRESSED";
  
  String jsonResponse = "{\"x\":\"" + String(analogRead(JOY_X_PIN)) + 
                       "\",\"button\":\"" + buttonText + "\"}";
  request->send(200, "application/json", jsonResponse);
}

void handleSetAngle(AsyncWebServerRequest *request) {
  if (request->hasParam("angle"))
    angle = request->getParam("angle")->value().toInt();
  request->send(200, "text/plain", "set success");
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "404 ERROR!");
}