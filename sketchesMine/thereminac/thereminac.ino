#include <Tone.h>

int I1, I2;
Tone myTone;

void setup() 
{
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  //myTone.begin(2);
}

void makeNoise()
{
  
}

void loop()
{
  I1 = analogRead(0);
  I2 = analogRead(1);
  //myTone.play(I1, 250);
  //while(myTone.isPlaying());
}

