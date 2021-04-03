#include <Servo.h>
// include the library, this is builtin not downloaded from 3rd party repositories

Servo myServo;
//make a servo object called myServo

void setup() 
{
  myServo.attach(4);
  // attach myServo object to pin 2 (pwm, talks to all servos digital or analog in theory)
  pinMode(13, OUTPUT);
  // set up the built in LED on an uno to flash
}

void loop() 
{
  myServo.write(0);
  // move the servo to 0°
  digitalWrite(13, HIGH);
  // turn the LED on
  delay(1000);
  // wait a second
  digitalWrite(13, LOW);
  // turn the LED off
  myServo.write(130);
  // move the servo to 130°
  delay(1000);
  myServo.write(90);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}
