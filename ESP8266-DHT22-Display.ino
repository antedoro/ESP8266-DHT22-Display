#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold9pt7b.h> 
#include <Fonts/FreeMono9pt7b.h> 

// OLED display width and height, in pixels
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Replace with your network credentials
const char* ssid = "YOUR_SSID"; 
const char* password = "YOUR_PASSWORD"; 

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

#define DHTPIN 14     // Digital pin connected to the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Updates DHT readings every 10 seconds
const long interval = 10000;  
unsigned long previousMillis = 0;    // Will store last time DHT was updated

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
      font-family: Arial;
      display: inline-block;
      margin: 0px auto;
      text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels {
      font-size: 1.5rem;
      vertical-align: middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP8266 DHT Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>
</body>
<script>
setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var data = JSON.parse(this.responseText);
      document.getElementById("temperature").innerHTML = data.temperature;
      document.getElementById("humidity").innerHTML = data.humidity;
    }
  };
  xhttp.open("GET", "/data", true);
  xhttp.send();
}, 10000);
</script>
</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();

  // Check I2C Address
  Serial.println("Scanning for I2C devices...");
  Wire.begin(D2, D1); // SDA on D2 (GPIO4), SCL on D1 (GPIO5)
  for (uint8_t i = 0; i < 128; i++) {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C device found at address 0x");
      if (i < 16) Serial.print("0");
      Serial.print(i, HEX);
      Serial.println(" !");
    }
  }
  
  // Initialize SSD1306 OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display(); // Show initial blank screen
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // New route for temperature and humidity data
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    String jsonResponse = "{\"temperature\":" + String(t) + ", \"humidity\":" + String(h) + "}";
    request->send(200, "application/json", jsonResponse);
  });

  server.begin();
}

void updateDisplay(float temperature, float humidity) {
  display.clearDisplay();

  // Set the font for the temperature and humidity
  display.setFont(&FreeSansBold9pt7b); // Set the custom font
  display.setTextSize(1); // Set text size to 1 for smaller appearance
  display.setTextColor(SSD1306_WHITE);
  
  // Display Temperature
  display.setCursor(0, 25);
  display.print("Temp: ");
  display.print(temperature);
  display.print(" C");

  // Display Humidity
  display.setCursor(0, 50);
  display.print("Hum:   ");
  display.print(humidity);
  display.print(" %");
  
  display.display(); // Refresh the display
}

void loop() {  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float newT = dht.readTemperature();
    float newH = dht.readHumidity();

    if (!isnan(newT)) {
      t = newT;
      Serial.print("Temperature: "); Serial.println(t);
    }
    if (!isnan(newH)) {
      h = newH;
      Serial.print("Humidity: "); Serial.println(h);
    }

    updateDisplay(t, h);
  }
}
