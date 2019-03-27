#include <Arduino.h>
//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include "fauxmoESP.h"

#define WIFI_SSID "dlink"
#define WIFI_PASS "12345abc"
#define SERIAL_BAUDRATE                 115200
#define LED_BUILTIN 9   //Esp32
//int LED_BUILTIN = 9;

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) {
    Serial.println("ON");
    digitalWrite(LED_BUILTIN, LOW); 
  } else {
    Serial.println("OFF");
    digitalWrite(LED_BUILTIN, HIGH);     
  }
}

void setup() {
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("FauxMo demo sketch");
    Serial.println("After connection, ask Alexa/Echo to 'turn <devicename> on' or 'off'");
    pinMode(LED_BUILTIN, OUTPUT); 
    // Wifi
    wifiSetup();

    // Fauxmo
    fauxmo.addDevice("kitchen light");
    //fauxmo.addDevice("pixels");
  //  fauxmo.onMessage(callback);
}

void loop() {
  fauxmo.handle();
}
