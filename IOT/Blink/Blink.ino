#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "MAX30100_PulseOximeter.h"

const char* ssid = "Galaxy";
const char* password = "00000000";
#define DEVICE_ID "15555000"

PulseOximeter pox;
float BPM, SpO2;
const int oneWireBus = 12;
OneWire oneWire(oneWireBus);
DallasTemperature tempsensor(&oneWire);
float tempC;

void setup() {

  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  // loop_new();
  tempsensor.begin();
  // pox_start();
}

void loop() {
  tempsensor.requestTemperatures();
  tempC = tempsensor.getTempCByIndex(0);
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  loop_new();
}

void loop_new() {
  HTTPClient http;
  WiFiClient wifiClient;

  String url = "http://192.168.89.136:5000/record";

  // Send GET request
  http.begin(wifiClient, url);
  http.addHeader("Content-Type", "application/json");
  // int httpCode = http.GET();
  // int tempC = 10;
  // int httpCode = http.POST("{\"temperature\":\"" + (String)tempC + "\", \"deviceid\":\"" + DEVICE_ID + "\"}");
  int httpCode = http.POST("{\"temperature\":\"" + (String)tempC + "\", \"bpm\":\"" + (String)BPM + "\", \"spo2\":\"" + (String)SpO2 + "\", \"deviceid\":\"" + DEVICE_ID + "\" }");

  if (httpCode > 0) {
    // Check for successful response
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Response: " + payload);
    }
  } else {
    Serial.println("Error in HTTP request");
  }

  http.end();

  delay(4000);
}

void pox_start() {
  if (!pox.begin()) {
    //     oled.clearDisplay();
    //     oled.setCursor(40, 32);
    //     oled.println("POX Error");
    //     oled.display();
    for (;;)
      ;
  } else {
    // oled.clearDisplay();
    // oled.setTextSize(1);
    // oled.setTextColor(1);
    // oled.setCursor(40, 32);
    // oled.println("POX SUCCESS");
    // oled.display();
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}