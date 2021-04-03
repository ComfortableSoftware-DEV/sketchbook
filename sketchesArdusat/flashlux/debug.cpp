#include "debug.h"
#include "settings.h"

//original avrdude error messages get copied to ram and overflow, wo use numeric codes.
void error1(int errno, unsigned char detail) {
  DEBUGP("error: ");
  DEBUGP(errno);
  DEBUGP(" detail: 0x");
  DEBUGPLN(detail,HEX);
}

void error(int errno) {
  DEBUGP("error" );
  DEBUGPLN(errno);
}


/* Helper Functions */

void setLED(int state) {
  digitalWrite(NOTIFY_LED, state);
}

