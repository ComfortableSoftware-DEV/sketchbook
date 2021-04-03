#include "debug.h"

#define INTTIME TSL2561_TIME_13MS
#define GAIN true

// 0x92 is the same as (0x49 << 1) and is the spaceboard I2C address for the TSL2561
#define ADDR 0x92
#define SDA_PORT PORTB
#define SDA_PIN 1
#define SCL_PORT PORTB
#define SCL_PIN 3

/** Wiring for Ardusat Space Board
 *  
 *  On a genuine Arduino (but not on a RedBoard, Seeeduino, etc.),
 *  there are 6 pins next to the USB port that are connected to the
 *  Atmel 16u2 USB-to-serial chip. To connect the sensor board to
 *  this chip's pins, wire it as follows:
 *  
 *  +---+---+---+
 *  | 5 | 3 | 1 |      <-- White and Yellow go here (3, 1)
 *  +---+---+---+
 *  | 6 | 4 | 2 |      <-- Black and Red go here (6, 2)
 *  +---+---+---+
 *  
 *  YELLOW (SDA) : Pin 1
 *  WHITE  (SCL) : Pin 3
 *  RED    (+5V) : Pin 2
 *  BLACK  (GND) : Pin 6
 */

// 0x72 is the same as (0x39 << 1) and is the default I2C address for the TSL2561
//#define ADDR 0x72
//#define SDA_PORT PORTC
//#define SDA_PIN 4
//#define SCL_PORT PORTC
//#define SCL_PIN 5

#define I2C_FASTMODE 1

#include <SoftI2CMaster.h>
#include "lum_sensor.h"

void lum_sensor_init() {
  DEBUGPLN(F("Initializing luminosity sensor..."));
  if (i2c_init()) {
    DEBUGPLN(F("Sensor initialized"));
  }

  lum_sensor_wake();

  i2c_start(ADDR | I2C_WRITE);
  if (!i2c_write(0x81)) {
    DEBUGPLN(F("Cannot address reg 1"));
  }
  if (!i2c_write((GAIN ? 0x10 : 0x00) + INTTIME)) {
     DEBUGPLN(F("Cannot set gain")); 
  }
  i2c_stop();
}

void lum_sensor_wake() {
  while (!i2c_start(ADDR | I2C_WRITE)) {
    DEBUGPLN(F("Device does not respond"));
    i2c_stop();
    delay(1000);
  }

  if (!i2c_write(0x80)) {
    DEBUGPLN(F("Cannot address reg 0"));
  }
  if (!i2c_write(0x03)) {
    DEBUGPLN(F("Cannot wake up")); // wake up
  }
  i2c_stop();
}

void lum_sensor_get_values(int* low0, int* high0, int* low1, int* high1) {
  i2c_start(ADDR | I2C_WRITE);
  i2c_write(0x8C);
  i2c_rep_start(ADDR | I2C_READ);
  *low0 = i2c_read(false);
  *high0 = i2c_read(true);
  i2c_stop();
  i2c_start(ADDR | I2C_WRITE);
  i2c_write(0x8E);
  i2c_rep_start(ADDR | I2C_READ);
  *low1 = i2c_read(false);
  *high1 = i2c_read(true);
  i2c_stop();
}

void lum_sensor_get_channels(int* chan0, int* chan1) {
  unsigned int low0, high0, low1, high1;

  lum_sensor_get_values(&low0, &high0, &low1, &high1);

  *chan0 = low0 + (high0 << 8);
  *chan1 = low1 + (high1 << 8);
}

unsigned int lum_sensor_get_lux() {
  unsigned int chan0, chan1;

  lum_sensor_get_channels(&chan0, &chan1);

  return lum_sensor_compute_lux(GAIN, INTTIME, chan0, chan1);
}

unsigned long lum_sensor_compute_lux(boolean gain, int intTime , unsigned long channel0, unsigned long channel1) {
  
  uint16_t clipThreshold;
  unsigned long chScale;
  
  switch (intTime) {
  case TSL2561_TIME_13MS: 
    clipThreshold = TSL2561_CLIPPING_13MS;
    chScale =  TSL2561_LUX_CHSCALE_TINT0;
    break;
  case TSL2561_TIME_101MS: 
    clipThreshold = TSL2561_CLIPPING_101MS;
    chScale = TSL2561_LUX_CHSCALE_TINT1;
    break;
  case TSL2561_TIME_402MS: 
    clipThreshold = TSL2561_CLIPPING_402MS;
    chScale = (1 << TSL2561_LUX_CHSCALE);
    break;
  }
  if (!gain) chScale = chScale << 4;

  /* Return MAX lux if the sensor is saturated */
  if ((channel0 > clipThreshold) || (channel1 > clipThreshold))
  {
    DEBUGPLN(F("Sensor is saturated"));
    return 32000;
  }

  channel0 = (channel0 * chScale) >> TSL2561_LUX_CHSCALE;
  channel1 = (channel1 * chScale) >> TSL2561_LUX_CHSCALE;
  

  /* Find the ratio of the channel values (Channel1/Channel0) */
  unsigned long ratio1 = 0;
  if (channel0 != 0) ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE+1)) / channel0;

  /* round the ratio value */
  unsigned long ratio = (ratio1 + 1) >> 1;

  unsigned int b, m;

  if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1T))
    {b=TSL2561_LUX_B1T; m=TSL2561_LUX_M1T;}
  else if (ratio <= TSL2561_LUX_K2T)
    {b=TSL2561_LUX_B2T; m=TSL2561_LUX_M2T;}
  else if (ratio <= TSL2561_LUX_K3T)
    {b=TSL2561_LUX_B3T; m=TSL2561_LUX_M3T;}
  else if (ratio <= TSL2561_LUX_K4T)
    {b=TSL2561_LUX_B4T; m=TSL2561_LUX_M4T;}
  else if (ratio <= TSL2561_LUX_K5T)
    {b=TSL2561_LUX_B5T; m=TSL2561_LUX_M5T;}
  else if (ratio <= TSL2561_LUX_K6T)
    {b=TSL2561_LUX_B6T; m=TSL2561_LUX_M6T;}
  else if (ratio <= TSL2561_LUX_K7T)
    {b=TSL2561_LUX_B7T; m=TSL2561_LUX_M7T;}
  else if (ratio > TSL2561_LUX_K8T)
    {b=TSL2561_LUX_B8T; m=TSL2561_LUX_M8T;}

  unsigned long temp;
  temp = ((channel0 * b) - (channel1 * m));

  /* Do not allow negative lux value */
  if (temp < 0) temp = 0;

  /* Round lsb (2^(LUX_SCALE-1)) */
  temp += (1 << (TSL2561_LUX_LUXSCALE-1));

  /* Strip off fractional portion */
  unsigned long lux = temp >> TSL2561_LUX_LUXSCALE;

  return lux;
}
