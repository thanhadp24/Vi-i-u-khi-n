#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

const char *ssid = "@@@@@@@";
const char *password = "aaaaaaaaa";

// Gas sensor configuration
const int GAS_PIN = A0;
const int SAMPLE_COUNT = 5;
const int GAS_HIGH_THRESHOLD = 850;
const int GAS_LOW_THRESHOLD = 800;

// Stepper motor configuration
int angle = 0;
const int stepPerRevolution = 2048;
Stepper myStepper = Stepper(2048, D3, D1, D2, D0);

// Gas sensor variables
bool isGasDetected = false;
int samples[SAMPLE_COUNT];
int sampleIndex = 0;

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>STEPPER MOTOR & GAS SENSOR</title>
    <style>
        body { font-family: Arial; text-align: center; margin: 20px; }
        .warning { color: red; font-weight: bold; font-size: 24px; }
        .safe { color: green; font-size: 24px; }
        .control-panel { margin: 20px; }
        input { font-size: 18px; margin: 10px; }
        button { font-size: 18px; padding: 5px 15px; }
    </style>
</head>
<body>
    <h1>Gas Sensor & Stepper Control</h1>
    <div>
        <p style="font-size: 25px"><b>Gas Status:</b> <span id="sensor_value" class="safe">Normal</span></p>
        <p style="font-size: 25px"><b>Gas Value:</b> <span id="gas_value">--</span></p>
    </div>
    <div class="control-panel">
        <label><b>Rotate for stepper motor (0-360):</b></label>
        <br>
        <input type="number" id="angle" min="0" max="360" value="">
        <br>
        <button id="set_angle-btn">Set Angle</button>
    </div>
    <script src="script.js"></script>
</body>
</html>
)rawliteral";

const char script_js[] PROGMEM = R"rawliteral(
const setAngle = () => {
    const angle = document.getElementById('angle').value;
    if(angle >= 0 && angle <= 360) {
        fetch("/motor?angle=" + angle);
        console.log("Setting angle:", angle);
    }
}

const readSensorValue = () => {  
    fetch("/sensor")
        .then(response => response.json())
        .then(data => {
            const statusElement = document.getElementById("sensor_value");
            const valueElement = document.getElementById("gas_value");
            
            const detected = data.isGas === 1;
            statusElement.textContent = detected ? 'GAS DETECTED!' : 'Normal';
            statusElement.className = detected ? 'warning' : 'safe';
            
            valueElement.textContent = data.value;
        });
}

document.getElementById('set_angle-btn').addEventListener('click', setAngle);
setInterval(readSensorValue, 1000);
)rawliteral";

void setup() {
    Serial.begin(115200);
    pinMode(GAS_PIN, INPUT);
    myStepper.setSpeed(18);

    // Initialize samples array
    for(int i = 0; i < SAMPLE_COUNT; i++) {
        samples[i] = analogRead(GAS_PIN);
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/script.js", handleScript);
    server.on("/sensor", handleSendSensorValue);
    server.on("/motor", handleSetAngle);
    server.onNotFound(handleNotFound);
    server.begin();
}

int getAverageReading() {
    int sum = 0;
    for(int i = 0; i < SAMPLE_COUNT; i++) {
        sum += samples[i];
    }
    return sum / SAMPLE_COUNT;
}

void updateGasStatus() {
    samples[sampleIndex] = analogRead(GAS_PIN);
    sampleIndex = (sampleIndex + 1) % SAMPLE_COUNT;
    
    int avgReading = getAverageReading();
    
    if(!isGasDetected && avgReading >= GAS_HIGH_THRESHOLD) {
        isGasDetected = true;
    } else if(isGasDetected && avgReading <= GAS_LOW_THRESHOLD) {
        isGasDetected = false;
    }
    
    Serial.printf("Gas Value: %d, Average: %d, Status: %s\n", 
                 samples[sampleIndex], avgReading, 
                 isGasDetected ? "DETECTED" : "Normal");
}

void loop() {
    static unsigned long lastUpdate = 0;
    static int step_count_delay = 0;
    const int step_max_delay = 50;

    // Update gas sensor readings
    updateGasStatus();

    // Handle stepper motor
    if (step_count_delay == 0) {
        int steps = map(angle, 0, 360, 0, 2048);
        myStepper.step(steps);
    }
    
    step_count_delay++;
    if(step_count_delay >= step_max_delay) step_count_delay = 0;
    
    delay(10);
}

void handleRoot(AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
}

void handleScript(AsyncWebServerRequest *request) {
    request->send_P(200, "text/javascript", script_js);
}

void handleSendSensorValue(AsyncWebServerRequest *request) {
    int currentValue = analogRead(GAS_PIN);
    String jsonResponse = "{\"isGas\":" + String(isGasDetected ? 1 : 0) + 
                         ",\"value\":" + String(currentValue) + "}";
    request->send(200, "application/json", jsonResponse);
}

void handleSetAngle(AsyncWebServerRequest *request) {
    if (request->hasParam("angle")) {
        angle = constrain(request->getParam("angle")->value().toInt(), 0, 360);
        Serial.printf("New angle set: %d\n", angle);
    }
    request->send(200, "text/plain", "OK");
}

void handleNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not Found");
}