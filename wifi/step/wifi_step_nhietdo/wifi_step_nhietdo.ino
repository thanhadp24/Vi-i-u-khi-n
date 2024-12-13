#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>
#include <DHT.h>

#define DHTPIN D7   // Chân kết nối DHT11 
#define DHTTYPE DHT11 // Loại cảm biến DHT11
DHT dht(DHTPIN, DHTTYPE);
//done nhưng lại hay bị sập 
const char *ssid = "@@@@@@@";
const char *password = "aaaaaaaaa";

float temperature;
float humidity;

int angle = 0;
const int stepPerRevolution = 2048;
Stepper myStepper = Stepper(2048, D3, D1, D2, D0);

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
  <html>
  <head>
    <title>STEPPER MOTOR & DHT11</title>
    <style>
      body { font-family: Arial; text-align: center; }
      .container { max-width: 600px; margin: 0 auto; padding: 20px; }
      .sensor-data { font-size: 25px; margin: 20px 0; }
      .control-panel { margin: 20px 0; }
    </style>
  </head>
  <body>
    <div class="container">
      <div class="sensor-data">
        <p><b>Nhiet do:</b> <span id="temperature">--</span>°C</p>
        <p><b>Do am:</b> <span id="humidity">--</span>%</p>
      </div>
      <div class="control-panel">
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
        document.getElementById("temperature").textContent = data.temperature;
        document.getElementById("humidity").textContent = data.humidity;
      });
  }

  document.getElementById('set_angle-btn').addEventListener('click', setAngle);
  setInterval(readSensorValues, 2000);
)rawliteral";

void setup() {
  Serial.begin(115200);
  dht.begin();
  myStepper.setSpeed(18);

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
int step_max_delay = 30;
float steps;

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
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
  delay(20);

}

void handleRoot(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", index_html);
}

void handleScript(AsyncWebServerRequest *request) {
  request->send_P(200, "text/javascript", script_js);
}

void handleSendSensorValue(AsyncWebServerRequest *request) {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  String jsonResponse = "{\"temperature\":\"" + String(temperature, 1) + 
                       "\",\"humidity\":\"" + String(humidity, 1) + "\"}";
  
  request->send(200, "application/json", jsonResponse);
}

void handleSetAngle(AsyncWebServerRequest *request) {
  if (request->hasParam("angle")) {
    angle = request->getParam("angle")->value().toInt();
    Serial.print("New angle set: ");
    Serial.println(angle);
  }
  request->send(200, "text/plain", "Angle set successfully");
}

void handleNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not Found");
}