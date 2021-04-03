#include "bootcomm.h"
#include "stk500.h"

const int rstPin = 20;

static inline void _board_init(void) {
  // PD3 is Serial RX, PD2 is Serial TX
  DDRD |= LEDS_ALL_LEDS | (1 << PD3) | AVR_RESET_LINE_MASK;
  PORTD |= AVR_RESET_LINE_MASK;
  PORTD |= (1 << PD2);
}

void bootcomm_toggle_reset() {
  pinMode(rstPin, OUTPUT);

  DEBUGPLN(F("Sending RESET Signal..."));
  // RESET pin goes HIGH
  digitalWrite(rstPin, HIGH);
  // Let the 100uF capacitor charge for 100ms
  delay(100);

  // Then send LOW / HIGH to reset
  digitalWrite(rstPin, LOW);
  delayMicroseconds(1000);
  digitalWrite(rstPin, HIGH);
  delay(10);
}

void bootcomm_start() {
  unsigned int major = 0;
  unsigned int minor = 0;

  Serial.begin(BOOT_BAUD);

  bootcomm_toggle_reset();

  stk500_getsync();
  stk500_getparm(Parm_STK_SW_MAJOR, &major);
  stk500_getparm(Parm_STK_SW_MINOR, &minor);
  DEBUGP(F("Bootloader Version: "));
  DEBUGP(major);
  DEBUGP(".");
  DEBUGPLN(minor);

  stk500_program_enable();

  // Atmel 328P is now ready to receive bytes
}

int bootcomm_burn(AVRMEM* tbuf) {
  stk500_loadaddr(tbuf->pageaddress >> 1);
  return stk500_paged_write(tbuf, tbuf->size);
}

void bootcomm_stop() {
  // Stop sending bytes to Atmel 328P, and reboot it so
  // it starts running the sketch we just burned
  stk500_disable();
  bootcomm_toggle_reset();
}

