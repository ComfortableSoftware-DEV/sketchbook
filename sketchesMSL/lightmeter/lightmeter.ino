//  https://github.com/adafruit/Adafruit_NeoPixel
// https://github.com/FastLED/FastLED for unsupported boards like nano
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define NUMLED 10
#define LOWLED 30
#define MEDIUMLED 50
#define HIGHLED 90
#define PIN 6

int lightlevel;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  strip.begin();
  strip.show();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  lightlevel = analogRead(A0);
  Serial.println(lightlevel);
  strip.clear();
  if (lightlevel >=0 && lightlevel <= LOWLED)
  {
    for (int I = 0; I < 3; I++)
    {
      strip.setPixelColor(I, strip.Color(255,255,255));
    }
      strip.show();
  }
  if (lightlevel >= LOWLED && lightlevel <= MEDIUMLED)
  {
    for (int I = 0; I < 6; I++)
    {
      strip.setPixelColor(I, strip.Color(255,255,255));
    }
    strip.show();
  }
  if (lightlevel >=MEDIUMLED && lightlevel <= HIGHLED)  
  {
    for (int I = 0; I < 9; I++)
    {
      strip.setPixelColor(I, strip.Color(255, 255,255));
    }
    strip.show();
  }
}
