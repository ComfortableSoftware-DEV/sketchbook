#include <Adafruit_NeoPixel.h>
// include the library to talk to the leds

#ifdef __AVR__
  #include <avr/power.h>
#endif
// if we are on an AVR chip (most older arduinos) we need to include the special power library

#define PIN 2 
// this is the control pin, it will hook to the center/din pin on the strips, it can be almost any pin on the board simply by using the number instead of 6

#define NUMLED 10
// how many individual leds are on our strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLED, PIN, NEO_GRB + NEO_KHZ800);
// define the object to represent the strip of leds, call it "strip", it has NUMLED leds, it will be run on pin PIN and is NEO_GRB type at NEO_KHZ800 speed
// see f)ile e)xamples a)dafruit n)neo pixels, strand test for other supported speeds and types

void setup() 
{
  // put your setup code here, to run once:
  strip.begin();
  //we are going to use the object "strip" we created above, so let's initialize it
  stripClear();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  cylonEye();
  rainbow();
}

void stripClear()
{
  for (int pos = 0; pos < NUMLED; pos++)
  {
    strip.setPixelColor(pos, strip.Color(0, 0, 0));
  }
  strip.show(); 
  // Initialize all pixels to 'off'
}

void cylonEye()
{
  for (int loopNum = 1; loopNum < 11; loopNum++)
  {
    for (int pos = 0; pos < NUMLED - 3; pos++)
    // we are going to count from 0 (the first led/pixel) to the number of leds on the strip - 3
    {
      for (int stripPos = 0; stripPos < NUMLED - 1; stripPos++)
      {
        if (stripPos == pos)
        {
          strip.setPixelColor(stripPos, strip.Color(32, 0, 0));
        }
        else if (stripPos == (pos + 1))
        {
          strip.setPixelColor(stripPos, strip.Color(255, 0, 0));
        }
        else if (stripPos == (pos + 2))
        {
          strip.setPixelColor(stripPos, strip.Color(32, 0, 0));
        }
        else
        {
          strip.setPixelColor(stripPos, strip.Color(0, 0, 0));
        }
      }
      strip.show();
      // tell the "strip" to do what we have set up
      delay(250);
      // sleep for 1/4 of a second for the effect to work (1 second is 1000 miliseconds, delay takes miliseconds as it's parameter)
    }
    for (int pos = NUMLED - 3; pos > -1; pos--)
    // we are going to count from 0 (the first led/pixel) to the number of leds on the strip - 3
    {
      for (int stripPos = NUMLED - 1; stripPos > -1; stripPos--)
      {
        if (stripPos == pos)
        {
          strip.setPixelColor(stripPos, strip.Color(32, 0, 0));
        }
        else if (stripPos == (pos + 1))
        {
          strip.setPixelColor(stripPos, strip.Color(255, 0, 0));
        }
        else if (stripPos == (pos + 2))
        {
          strip.setPixelColor(stripPos, strip.Color(32, 0, 0));
        }
        else
        {
          strip.setPixelColor(stripPos, strip.Color(0, 0, 0));
        }
      }
      strip.show();
      // tell the "strip" to do what we have set up
      delay(250);
      // sleep for 1/4 of a second for the effect to work (1 second is 1000 miliseconds, delay takes miliseconds as it's parameter)
    }
  }
}

void rainbow()
{
  stripClear();
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.setPixelColor(1, strip.Color(255, 255, 0));
  strip.setPixelColor(2, strip.Color(255, 255, 255));
  strip.setPixelColor(3, strip.Color(0, 255, 255));
  strip.setPixelColor(4, strip.Color(255, 0, 255));
  strip.setPixelColor(5, strip.Color(0, 0, 255));
  strip.setPixelColor(6, strip.Color(0, 255, 0));
  strip.setPixelColor(7, strip.Color(20, 0, 0));
  strip.setPixelColor(8, strip.Color(0, 32, 32));
  strip.setPixelColor(9, strip.Color(255, 127, 32));
  strip.setPixelColor(9, strip.Color(64, 0, 127));
  strip.show();
  delay(5000);
  stripClear();
}

