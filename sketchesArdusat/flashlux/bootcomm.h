#ifndef __BOOTCOMM_H__
#define __BOOTCOMM_H__

#include "stk500.h"
#include "debug.h"

// The speed at which this 16u2 is going to communicate with the 328p's bootloader
#define BOOT_BAUD 115200

/** Pin that can reset the main MCU. */
#define AVR_RESET_LINE_PORT PORTD
#define AVR_RESET_LINE_DDR DDRD
#define AVR_RESET_LINE_PIN PIND
// PD7 is the pin connected to 328P_DTR. Its value is 7 on an Uno, so the mask will be 0b10000000
// See https://rheingoldheavy.com/wp-content/uploads/2015/09/UNO_ATMEGA16U2_Subsystem.png
#define AVR_RESET_LINE_MASK (1 << PD7)

#define LEDS_LED1        (1 << 5)
#define LEDS_LED2        (1 << 4)
#define LEDS_ALL_LEDS    (LEDS_LED1 | LEDS_LED2)

void bootcomm_toggle_reset();
void bootcomm_start();
int bootcomm_burn(AVRMEM* tbuf);
void bootcomm_stop();

#endif
