#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAutoWiFiConfig.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>

#define WIFI_LED 2
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// Distance sensor pins
#define TRIGGER_PIN 18
#define ECHO_PIN 19
float duration, distance;


#define FIRMWARE_VERSION "0.3"

void setup()   {
  Serial.begin(9600);
  delay(200);

  // Setup 
  pinMode(TRIGGER_PIN, OUTPUT);  
	pinMode(ECHO_PIN, INPUT);  

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // 0x3C = 0x78 >> 1 (Arduino's )
  // init done
  
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
  WiFi.setHostname("deskdist");

  if (ESPAutoWiFiConfigSetup(WIFI_LED, false, 0)) { // check if we should start access point to configure WiFi settings
    return; // in config mode so skip rest of setup
  }

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}

void loop() {

  if (ESPAutoWiFiConfigLoop()) {  // handle WiFi config webpages
    return;  // skip the rest of the loop until config finished
  }
  
  ArduinoOTA.handle();
  distance = readDistance();
  printScreenInformation();
  delay(100);
}
void printScreenInformation(){  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Dist:"); display.print(distance); display.println(" cm");
  display.print("IP:"); display.println(WiFi.localIP());
  display.print("Ver:"); display.println(FIRMWARE_VERSION);
  display.display();
}

float readDistance(){
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  return (duration*.0343)/2;  
}
