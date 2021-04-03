#include <ADCTouch.h>

int ref0;     //reference value to remove offset
int read0;
int val0;

void setup() 
{
  ref0 = ADCTouch.read(A0, 1000); // ADCTouch(pin, numberOfSamples)
  Serial.begin(115200); // Serial.begin(baudrate)
 } 

void loop() 
{
  read0 = ADCTouch.read(A0, 100);

  val0 = read0 - ref0;

  delay(500);
  Serial.print(ref0, ", ");
  Serial.print(read0, ", ");
  Serial.println(val0);
}
