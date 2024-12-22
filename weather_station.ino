/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/
*********/

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "Students";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

#define DHTPIN 5     // Digital pin connected to the DHT sensor

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;  
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    /* Global styles */
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      background: #f5f5f5;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    /* Main container */
    .container {
      background-color: white;
      padding: 30px;
      border-radius: 15px;
      box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
      text-align: center;
      width: 100%;
      max-width: 500px;
    }

    h2 {
      font-size: 2.5rem;
      color: #333;
      margin-bottom: 20px;
    }

    /* Icon and text styling */
    .icon {
      font-size: 2.5rem;
    }

    /* Red thermometer icon for temperature */
    .temperature .icon {
      color: #e53935; /* Red color */
    }

    /* Blue tint icon for humidity */
    .humidity .icon {
      color: #1e88e5; /* Blue color */
    }

    .data {
      font-size: 2.5rem;
      font-weight: bold;
      color: #333;
    }

    .units {
      font-size: 1.2rem;
      color: #555;
    }

    /* Label styling */
    .dht-labels {
      font-size: 1.5rem;
      color: #777;
      display: block;
      margin-bottom: 5px;
    }

    /* Subtle border and padding for sections (without border-left) */
    .section {
      margin-bottom: 20px;
      padding: 10px;
      border-radius: 10px;
      background-color: #f9f9f9;
    }

    /* Smooth transitions for updates */
    .data, .units {
      transition: color 0.3s ease;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>IISER Pune Weather Station</h2>
    <div class="section temperature">
      <div class="dht-labels">Temperature</div>
      <div class="icon">
        <i class="fas fa-thermometer-half"></i>
      </div>
      <div class="data" id="temperature">%TEMPERATURE%</div>
      <div class="units">&deg;C</div>
    </div>
    <div class="section humidity">
      <div class="dht-labels">Humidity</div>
      <div class="icon">
        <i class="fas fa-tint"></i>
      </div>
      <div class="data" id="humidity">%HUMIDITY%</div>
      <div class="units">%</div>
    </div>
  </div>
</body>
</html>)rawliteral";


// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}
void connectToWiFi();
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();
  
  // Connect to Wi-Fi
  connectToWiFi();

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });

  // Start server
  server.begin();
}

void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;

    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      t = newT;
      Serial.println(t);
    }

    // Read Humidity
    float newH = dht.readHumidity();
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    } else {
      h = newH;
      Serial.println(h);
    }
  }

  // Check and reconnect Wi-Fi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Attempting to reconnect...");
    connectToWiFi();
  }
}

void connectToWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid);
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
      delay(500);
      Serial.print(".");
      retryCount++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nConnected to Wi-Fi");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nFailed to reconnect to Wi-Fi");
    }
  }
}
