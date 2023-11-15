#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESPAutoWiFiConfig.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define WIFI_LED 2
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define SSD1306_LCDHEIGHT 64

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


#define FIRMWARE_VERSION "0.1"
const int trigPin = 18;  
const int echoPin = 19; 
float duration, distance;

void setup()   {
  Serial.begin(9600);
  delay(200);

  // Setup 
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  


  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // 0x3C = 0x78 >> 1 (Arduino's )
  // init done
  
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

  if (ESPAutoWiFiConfigSetup(WIFI_LED, false, 0)) { // check if we should start access point to configure WiFi settings
    return; // in config mode so skip rest of setup
  }
}

void loop() {

  if (ESPAutoWiFiConfigLoop()) {  // handle WiFi config webpages
    return;  // skip the rest of the loop until config finished
  }

  distance = readDistance();
  printScreenInformation();
  delay(100);
}
void printScreenInformation(){  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Dist:"); display.println(distance);
  display.print("IP:"); display.println(WiFi.localIP());
  display.print("Ver:"); display.println(FIRMWARE_VERSION);
  display.display();
}

float readDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return (duration*.0343)/2;  
}
