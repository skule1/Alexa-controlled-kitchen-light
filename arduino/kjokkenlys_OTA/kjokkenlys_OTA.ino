#include <Arduino.h>
//#ifdef ESP32
//    #include <WiFi.h>
//#else
    #include <ESP8266WiFi.h>
    #include <ESP8266mDNS.h>
    #include <WiFiUdp.h>
    #include <ArduinoOTA.h>    
//#endif
#include "fauxmoESP.h"
#include <EEPROM.h>

// Rename the credentials.sample.h file to credentials.h and 
// edit it according to your router configuration
#include "credentials.sample.h"

fauxmoESP fauxmo;
int p;
boolean first=true;
int teller = 0;

// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE     115200

#define pwm_port         D5


#define ID_kitchen           "kitchen light"

// -----------------------------------------------------------------------------

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

void setup() {
EEPROM.begin(5);
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();


    // LEDs

    pinMode(2, OUTPUT);
    pinMode(pwm_port,OUTPUT);


    // Wifi
 WiFi.disconnect();
    teller = 0;
 String    ssid = "COMFAST";
 String    password = "12345abc";
    Serial.print("Prøver dlink ..");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while ((WiFi.waitForConnectResult() != WL_CONNECTED) && (teller++ < 10))
    {
      Serial.print(".");
      delay(500);
    };
    Serial.println();
 
  if (teller >= 10)
  {
    teller = 0;
    WiFi.disconnect();
    ssid = "dlink";
    password = "12345abc";
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Prøver 2.4G ..");
    WiFi.begin(ssid, password);
    while ((WiFi.waitForConnectResult() != WL_CONNECTED) && (teller++ < 10)) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
  };
  if (teller >= 10)
  {
    WiFi.disconnect();
    teller = 0;
    ssid = "COMFAST";
    password = "12345abc";
    Serial.print("Prøver COMFAST..");
    WiFi.begin(ssid, password);
    while ((WiFi.waitForConnectResult() != WL_CONNECTED) && (teller++ < 10))
    {
      Serial.print(".");
      delay(500);
    };
    Serial.println();
  };

    // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // You can use different ways to invoke alexa to modify the devices state:
    // "Alexa, turn yellow lamp on"
    // "Alexa, turn on yellow lamp
    // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

    // Add virtual devices
    fauxmo.addDevice(ID_kitchen);
    Serial.println("first: "+String(EEPROM.read(1)));

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, ID_kitchen)==0) {
          if (first) {
            value = EEPROM.read(1);
            Serial.println("First "+String(value));
          }
         else
         {
          EEPROM.write(1,value);
              EEPROM.commit();
            Serial.println("Not First "+String(value));          
         }
          first=false;
           int g=1024-map(value,0,255,0,1024);
          Serial.println(g);
          if (state) 
          {
          digitalWrite(2,LOW);
          analogWrite(pwm_port,g);
          }
          else 
          {
            digitalWrite(2,HIGH);
            digitalWrite(pwm_port,HIGH);
          }
           Serial.print(state);
           Serial.print(" : ");
           Serial.println(value);

        } 
        

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
    });

}

void loop() {
  ArduinoOTA.handle();
    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();

    // This is a sample code to output free heap every 5 seconds
    // This is a cheap way to detect memory leaks
    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }

    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_YELLOW, true, 255);

}
