#include <Tone.h>

Tone myTone1;
Tone myTone2;
unsigned int lights = 4;
unsigned long int lightTime = 0;

void setup()
{
  myTone1.begin(12);
  myTone2.begin(13);
  DDRD = B11111100;
  lightTime = millis() + 500;
}

void checkLights()
{
  if (millis() > lightTime)
  {
    lights++;
    lightTime = millis() + 500;
    if ((lights < 4) || (lights > 255))
    {
      lights = 4;
    }
    PORTD = lights;
  }
}

void loop()
{
  while (analogRead(5) < 100)
  {
    checkLights();
  }
  PORTD = lights;
  myTone1.play(440, 500);
  myTone2.play(880, 500);
  while (myTone1.isPlaying())
  {
    checkLights();
  }
  myTone1.play(220, 500);
  myTone2.play(1100, 500);
  while (myTone1.isPlaying())
  {
    checkLights();
  }
}

