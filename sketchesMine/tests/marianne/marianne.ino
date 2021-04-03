#include <Adafruit_NeoPixel.h>
#include <Goertzel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 7
#define PIN2 4
//#define NUMLED 127
//#define NUMLED 30
#define NUMLED 15

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLED, PIN, NEO_RGB + NEO_KHZ800);
// Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMLED, PIN2, NEO_RGB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

int sensorPin = A0;
int led = 13;

// ideally an integer of SAMPLING_FREQUENCY/N to center the bins around your content so if you're
// looking for 700hz, frequencies below and above it equally contribute. Read up on Kevin's article 
// for more info.
// Nyquist says the highest frequency we can target is SAMPLING_FREQUENCY/2 
const float TARGET_FREQUENCY = 700; 

// if you're trying to detect several different drum hits all within low frequency like
// ~100-200hz you'll need a small bin size like 25 or 50 to distinguish them.
// If however you're just trying to find ANY bass hit you might want something
// basically equal to frequency youre looking for like ~100
 
// If Im detecting a frequency much higher with no care about nearby tones, like 2000hz
// Ill set to a round divisor like 200 So 1900 to 2100 could trigger, but not less or more
// Max is 200 as we have limited ram in the Arduino, and sampling longer would make us less 
// responsive anyway
const int N = 100;   

// This is what will trigger the led. Its INCREDIBLY squishy based on volume of your source, 
// frequency, etc. You'll just need to get in your environment and look at the serial console
// to start. Then pick something that triggers pleasantly to your eye.
const float THRESHOLD = 4000; 

// Again, the highest frequency we can target is SAMPLING_FREQUENCY/2. So Since Arduino is 
// relatively slow in terms of audio, we sample literally as fast as we can
// This is generally around ~8900hz for a 16mhz Arduino and 4400hz for an 8mhz Arduino.
// User nicola points out these rates are for stock arduino firmware and that on a board 
// by board basis you can juice the adc rates. For Arduino Uno you could move that rate up to 
// 22khz by adding somthing like this to your setup:
//  _SFR_BYTE(ADCSRA) |=  _BV(ADPS2); // Set ADPS2
//  _SFR_BYTE(ADCSRA) &= ~_BV(ADPS1); // Clear ADPS1
//  _SFR_BYTE(ADCSRA) &= ~_BV(ADPS0); // Clear ADPS0
const float SAMPLING_FREQUENCY = 8900; 

Goertzel goertzel = Goertzel(TARGET_FREQUENCY, N, SAMPLING_FREQUENCY);

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'

  pinMode(led, OUTPUT);     
//  Serial.begin(9600); 
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip2.Color(0, 255, 255), 50); // Red

  colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip2.Color(255, 0, 255), 50); // Green
  
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  //colorWipe(strip2.Color(255, 255, 0), 50); // Blue
  
  colorWipe(strip.Color(255, 0, 255), 50); // purple
  //colorWipe(strip2.Color(0, 255, 0), 50); // purple
  
  colorWipe(strip.Color(255, 255, 255), 50);
  //colorWipe(strip2.Color(0, 0, 0), 50);
  
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip2.Color(255, 255, 255), 50); // White

  theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip2.Color(0, 127, 127), 50); // Red
  
  theaterChase(strip.Color(255, 0, 255), 50); // Blue
  //theaterChase(strip2.Color(0, 255, 0), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  //theaterChaseRainbow(50);
  goertzel.sample(sensorPin); //Will take n samples
  
  float magnitude = goertzel.detect();  //check them for target_freq
  
  if(magnitude>THRESHOLD) //if you're getting false hits or no hits adjust this
    digitalWrite(led, HIGH); //if found, enable led
  else
    digitalWrite(led, LOW); //if not found, or lost, disable led
    
//  Serial.println(magnitude);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    strip2.setPixelColor(strip2.numPixels() - i, c);
    strip2.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
      strip2.setPixelColor(strip2.numPixels() - i, Wheel((i+j) & 255));
    }
    strip.show();
    strip2.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      strip2.setPixelColor(strip2.numPixels() - i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    strip2.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
        strip2.setPixelColor(i+q, 255 - c);    //turn every third pixel on
      }
      strip.show();
      strip2.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
        strip2.setPixelColor(i+q, 0);        //turn every third pixel off
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
uint32_t Wheel(byte WheelPos) {
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

