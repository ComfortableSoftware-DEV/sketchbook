#include <volume2.h>

Volume vol;

int read0, read1, read2;
int off0, off1, off2;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  read0 = analogRead(A0);
  Serial.println(read0);
  vol.tone((read0 * 4) + 100, PWM_25, 255);
  //delay(10000);
  
}
