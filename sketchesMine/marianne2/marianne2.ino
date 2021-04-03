#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define myRed 0
#define myGreen 20
#define myBlue 255
#define PIN PB3 // for attiny85
#define NUMLED 36
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLED, PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  // put your setup code here, to run once:
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(A2));
}

int myStrip;

void loop() 
{
  // put your main code here, to run repeatedly:
  for (myStrip = 0; myStrip < 12; myStrip++)
  {
    int numLEDs = random(4);
    //Serial.println(numLEDs);
    if ((myStrip % 2) == 0) // switch directions by changing 1/0
    {
      for (int myIndex = 0; myIndex < 3; myIndex++)
      {
        strip.setPixelColor((myStrip * 3) + myIndex, 0);
      }
      strip.show();
      //delay(1000);
      for (int myIndex = numLEDs; myIndex > 0; myIndex--)
      {
        strip.setPixelColor(((myStrip * 3) + myIndex - 1), strip.Color(myRed, myGreen, myBlue));
      }
      strip.show();
      //delay(1000);
    }
    else
    {
      for (int myIndex = 0; myIndex < 3; myIndex++)
      {
        strip.setPixelColor((myStrip * 3) + myIndex, 0);
      }
      strip.show();
      //delay(1000);
      for (int myIndex = (3 - numLEDs); myIndex < 3; myIndex++)
      {
        strip.setPixelColor(((myStrip * 3) + myIndex), strip.Color(myRed, myGreen, myBlue));
      }
      strip.show();
      //delay(1000);
    }
  }
  delay(50);
}
