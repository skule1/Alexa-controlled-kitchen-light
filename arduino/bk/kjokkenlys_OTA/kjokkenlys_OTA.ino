/*
 * This is a basic example on how to use Espalexa and its device declaration methods.
 */ 

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

 /*
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
*/

#include <Espalexa.h>
#define pwmport 5
#define statusport 2
// prototypes
boolean connectWifi();

//callback functions
//void firstLightChanged(uint8_t brightness);
//void secondLightChanged(uint8_t brightness);
//void thirdLightChanged(uint8_t brightness);

// Change this!!
const char* ssid = "dlink";
const char* password = "12345abc";

boolean wifiConnected = false;

Espalexa espalexa;

//EspalexaDevice* device3; //this is optional

void setup()
{
/*
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
*/
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");




    pinMode(pwmport,OUTPUT);
  pinMode(statusport,OUTPUT);    

  Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT); 
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    
    // Define your devices here. 
    espalexa.addDevice("kitchen light", firstLightChanged); //simplest definition, default state off
//    espalexa.addDevice("Light 2", secondLightChanged, 255); //third parameter is beginning state (here fully on)
    
//    device3 = new EspalexaDevice("Light 3", thirdLightChanged); //you can also create the Device objects yourself like here
 //  espalexa.addDevice(device3); //and then add them
  //  device3->setValue(128); //this allows you to e.g. update their state value at any time!

    espalexa.begin();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
    ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());



}
 
void loop()
{
    ArduinoOTA.handle();
   espalexa.loop();
   delay(1);
}

//our callback functions
void firstLightChanged(uint8_t brightness) {
    Serial.print("Device 1 changed to ");
    
    //do what you need to do here


    //EXAMPLE
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
 //    analogWrite(pwmport,1024-map(brightness,0,255,0,1024));
      analogWrite(statusport,1024-map(brightness,0,255,0,1024));  
        digitalWrite(LED_BUILTIN, LOW); 
    }
    else  {
      Serial.println("OFF");
      Serial.println(brightness);
      analogWrite(pwmport,1024-map(brightness,0,255,0,1024));
   //   analogWrite(statusport,1024-map(brightness,0,255,0,1024));  
        digitalWrite(LED_BUILTIN, HIGH); 
    }
}

void secondLightChanged(uint8_t brightness) {
  //do what you need to do here
}

void thirdLightChanged(uint8_t brightness) {
  //do what you need to do here
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
