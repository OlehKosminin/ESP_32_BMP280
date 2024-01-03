

// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <SPI.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)

Adafruit_BMP280 bmp;

const char *ssid = "Olga_VED";
const char *password = "Olegator324";


AsyncWebServer server(80);



String readBMP280Temperature() {
  // Read temperature as Celsius (the default)
  float t = bmp.readTemperature();
  // Convert temperature to Fahrenheit
  //t = 1.8 * t + 32;
  if (isnan(t)) {
    Serial.println("Failed to read from BMP280 sensor!");
    return "";
  } else {
    Serial.println(t);
    return String(t);
  }
}

String readBMP280Pressure() {
  float h = bmp.readPressure();
  if (isnan(h)) {
    Serial.println("Failed to read from BME280 sensor!");
    return "";
  } else {
    Serial.println(h);
    return String(h);
  }
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  bool status;
  status = bmp.begin(0x76);

  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readBMP280Temperature().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", readBMP280Pressure().c_str());
  });

  // Start server
  server.begin();
}

void loop() {
}
