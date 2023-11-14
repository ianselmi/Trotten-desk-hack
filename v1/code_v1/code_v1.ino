#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define SSD1306_LCDHEIGHT 64

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int trigPin = 19;  
const int echoPin = 18; 
float duration, distance;

void setup()   {
  Serial.begin(9600);
  delay(200);

  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  


  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // 0x3C = 0x78 >> 1 (Arduino's )
  // init done
  
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

}


void loop() {
  distance = readDistance();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Dist:");
  display.println(distance);
  display.display();
  delay(100);
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
