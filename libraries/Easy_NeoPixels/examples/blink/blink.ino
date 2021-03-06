/*

Blink bright white light from a single Neopixel!

(Don't forget to install [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) to use EasyNeoPixels.)
*/

#include <EasyNeoPixels.h>

void setup() {
    // setup for one NeoPixel attached to pin 13
    setupEasyNeoPixels(13, 1);
}

void loop() {
    // turn the NeoPixel ON
    writeEasyNeoPixel(0, HIGH);
    delay(500);
    // turn the NeoPixel OFF
    writeEasyNeoPixel(0, LOW);
    delay(500);
}
