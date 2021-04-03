#include <SPI.h>
#include "Ucglib.h"
Ucglib_ILI9163_18x128x128_HWSPI ucg(/*cd=*/ 9 , /*cs=*/ 10, /*reset=*/ 8);  /* HW SPI Adapter */ // little screens


void setup() {
  // put your setup code here, to run once:
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.setFont(ucg_font_ncenR14_hr);
  ucg.clearScreen();

}

void loop() {
  // put your main code here, to run repeatedly:

}
