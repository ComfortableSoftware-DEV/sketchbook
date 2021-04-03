#ifndef __ARDUSAT_DEBUG_H__
#define __ARDUSAT_DEBUG_H__

#define DEBUG_BAUD 115200

#include <Arduino.h>

#define DEBUGPLN Serial.println
#define DEBUGP Serial.print

void error1(int errno, unsigned char detail);
void error(int errno);

void setLED(int state);

#endif
