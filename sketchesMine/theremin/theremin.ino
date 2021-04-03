#include <Tone.h>
// include the tone library so we can use it

int I1, I2;
// define the variables we are using

Tone myTone;
// define object to use for the tone library

void setup() 
{
  myTone.begin(2);
}
// define the stuf that is run once at startup and never run again

void loop()
{
  I1 = analogRead(0);
  // read an integer from analog pin 0
  myTone.play(I1 * 1.7, 255);
  // start playing the tone
  while (myTone.isPlaying()) 
    {}
    // put an empty block so nothing is done
  // 2wait until it is done playing
}

