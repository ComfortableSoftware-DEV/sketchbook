#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
// PIN PB12 // this is the control pin, it can be almost any pin on the board simply by using the number instead of 6
//#define PIN PB3 // for attiny85
//#define NUMLED 127
//#define NUMLED 54
//#define NUMLED 30
#define NUMLED 15
//#define NUMLED 10
//#define NUMLED 36
//#define NUMLED 300
#define FLASHPIN 13

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLED, PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLED, PIN, NEO_RGB + NEO_KHZ800);
// *************************** notice change from GRB to RGB in line above
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across (somewhat useful with these LEDs)
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input (pretty useless with these LEDs)
// and minimize distance between Arduino and first pixel.  Avoid connecting (ALWAYS A GOOD IDEA to keep everything as short as possible, inches are not a big deal, feet are)
// on a live circuit...if you must, connect GND first. (ALWAYS WORK DEAD UNLESS YOU MUST WORK LIVE FOR EXCEPTIONAL CIRCUMSTANCES)

bool LEDOUT;

void setup() { // run once at the start of the program executiuon

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(FLASHPIN, OUTPUT);
  digitalWrite(FLASHPIN, HIGH);
  delay(1000);
  LEDOUT = LOW;
  digitalWrite(FLASHPIN, LEDOUT);
}

void flashled()
{
  LEDOUT = ! LEDOUT;
  digitalWrite(FLASHPIN, LEDOUT);
}

void dly(int DLA)
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
    strip.show();
    delay(50);
  }
  delay(DLA);
}
void loop() {  // run from top to bottom forever after the setup()
  
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 100); // Red
  dly(1000);
  flashled();
  
  colorWipe(strip.Color(0, 255, 0), 100); // Green
  dly(1000);
  flashled();
  
  colorWipe(strip.Color(0, 0, 255), 100); // Blue
  dly(1000);
  flashled();
  
  colorWipe(strip.Color(255, 0, 255), 100); // purple
  dly(1000);
  flashled();
  
  colorWipe(strip.Color(255, 255, 255), 100);
  dly(1000);
  //delay(25000);
  flashled();
  
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 100); // White
  dly(1000);
  flashled();

  theaterChase(strip.Color(127, 0, 0), 100); // Red
  dly(1000);
  flashled();
  
  theaterChase(strip.Color(255, 0, 255), 100); // purple
  dly(1000);
  flashled();

  rainbow(25);
  dly(1000);
  flashled();
  rainbowCycle(25);
  dly(1000);
  flashled();
  theaterChaseRainbow(50);
  dly(1000);
  flashled();
  //Wheel(50);
  //dly(1000);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<50; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
