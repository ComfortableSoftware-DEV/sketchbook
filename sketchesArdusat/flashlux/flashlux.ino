#include "lum_sensor.h"
#include "bootcomm.h"
#include "stk500.h"

#include "settings.h"
#include "handshake.h"
#include "light_signal.h"
#include "light_stream.h"
#include "hamming.h"

/**
  * Helpful Documentation
  *
  * Adapting the stk500 protocol to work inside the Arduino:
  * - stk500 protocol:  http://www.atmel.com/Images/doc2525.pdf
  * - avrdude:          https://github.com/kcuzner/avrdude
  * - bootdrive:        https://github.com/osbock/Baldwisdom/tree/master/BootDrive
  *
  * Creating space on the 16u2 for USB device and custom firmware:
  * - hoodloader2:      https://github.com/NicoHood/HoodLoader2/wiki
  *
  * The default bootloader on Arduino Uno (goes on the Atmel 328p chip, NOT the 16u2):
  * - optiboot:         https://github.com/Optiboot/optiboot/
  *
  * Using available pins on the 16u2 for I2C:
  * - arduino scratch:  https://rheingoldheavy.com/arduino-from-scratch-part-8-atmega16u2-subsystem/
  * - softi2cmaster:    https://github.com/todbot/SoftI2CMaster
  *
  * Using decompression algo that fits on an Arduino:
  * - fastlz:           http://fastlz.org/index.html
  *                     https://github.com/ariya/FastLZ
  *
  * Detecting and correcting errors in streaming data:
  * - hamming codes:    https://bitbucket.org/bullakio/hamming-8-4-arduino/src
  *                     https://www.youtube.com/watch?v=TYwrHiQ2-G4
  *
  * Light sensors capable of getting data in for "flashing" the Atmel 328p:
  * - tcs34725 (RGB):   https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf
  *                     https://github.com/leo3linbeck/spark-softi2c-tcs347265/tree/master/src
  * - tsl2561 (Lum):    https://cdn-shop.adafruit.com/datasheets/TSL2561.pdf
  *                     https://github.com/felias-fogg/SoftI2CMaster/tree/master/examples/TSL2561Soft
  */

avrmem transferBuffer;
//unsigned char mempage[128];

Handshake handshake;
LightSignal lightSignal;
LightStream lightStream;
Hamming hamm(&lightStream);

unsigned char inBit = 0;
unsigned char inByte = 0, outByte = 0;
char inByteBit = 7;
bool handshakeCompleteShown = false;

void setup(void) {
  // Set up buffer to point to 128 bytes of allocated memory
//  transferBuffer.buf = &mempage[0];

  /* Normally, an Arduino UNO doesn't need this, but since we're using
   * hoodloader2 and USB, we need to wait for the Serial communication
   * line to be ready:
   */
  while (!Serial);
  Serial.begin(115200);

  lum_sensor_init();

  handshake.reset();
}

void loop (void) {
  // The world waits for the signal edge...
  lightSignal.signalEdgeWait();

  // Immediately get current lux reading--should take about 13ms
  lightSignal.setLux(lum_sensor_get_lux());

  // Increment signal edge time to next edge
  lightSignal.signalEdgeAdvance();

  if (handshake.isEstablished()) {
    if (!handshakeCompleteShown) {
      DEBUGPLN(F("handshake complete."));
      handshakeCompleteShown = true;
    }
    inBit = lightSignal.getBit();
    DEBUGP(inBit == 1 ? '1' : '0');
    inByte |= (inBit << (inByteBit--));
    if (inByteBit == -1) {
      lightStream.receiveByte(inByte);
      inByteBit = 7;
      DEBUGPLN();
      inByte = 0;
    }
    if (hamm.isMessageReady()) {
      hamm.read(&outByte);
      DEBUGP(F("outByte: "));
      if (outByte >= 32 && outByte <= 126) {
        DEBUGP((char)outByte);
      } else {
        DEBUGP("~");
      }
      DEBUGP(F(" "));
      DEBUGPLN(outByte, DEC);
    }
  } else {
    handshake.establishStep(&lightSignal);
  }
}


