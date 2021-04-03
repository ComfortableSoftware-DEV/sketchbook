// <-- starts a comment, or note to yourself about your code, 
// this helps you remember what is going on, 
// especially after you have put this piece of code aside for a while

/*
 * you can also use this format wfor multiple line comments
 * and the IDE will add a new line to the comment each time you press enter
 */

// comments can be at the end of another line like the following

int ledPin = 13; // integer variable called ledPin is set to 13
int switchPin = 2; // another called switchPin set to 2

/* 
 *  the first word is the type of variable, commonly used types are
 *  bool a boolean HIGH/LOW true/false etc.
 *  int an integer, or whole number, no decimal point
 *  float a real number with a decimal point
 *  char one or more characters of text
 *  void = nothing
 *  = means "set equal to" and puts the value right of the = into the box named by the 2nd word
 */


void setup() 
{ // a function that returns nothing, and must return nothing, expects nothing (), and must expect nothing, it must be called setup()
  pinMode(ledPin, OUTPUT); // which set the pin numbered pin ledPin (13) as an output (to an LED)
  pinMode(switchPin, INPUT_PULLUP); // and pin the pin numbered switchPin to an input with a default state of HIGH
}

/*
 * this is not hard required, and is a good idea to always include, even if it is empty {}
 * this special function is only run at power up or reset, and only once, anything you need to guarantee the value of goes before anything 
 * else in your code has a chance to do anything, belongs here, this is why it takes no arguments, and returns nothing (void)
 */


int checkSwitch()                           // this function returns an integer, and expects no arguments, this name is chosen by us, use something meaningful
{ 
  int switchValue = digitalRead(switchPin); // make a temporary variable of type int(eger) which is the value read from a digital input pin
  return switchValue;                       // and return that value to the caller
}


void loop() // always required, always expect nothing, and returns nothing, this repeats as long as the board has power, restarts when power is restored, etc.
{
  digitalWrite(ledPin, HIGH);           // turn on the LED hooked to pin 13 we have called ledPin
  delay(300);                           // do absolutely nothing for 0.3 seconds
  digitalWrite(ledPin, LOW);            // now turn the LED off
  delay(1000);                          // and do nothing for a full second
  bool switchPressed = checkSwitch();   // set a temporary variable of type bool(ean) from the function we called checkSwitch
  if (switchPressed == true)            // a test of a variable, "if something is" something else, must be "if (condition/conditions) { do things } (see flow control exercise on the MSL notion pages)"
  {
    delay(3000); // do nothing for 3 seconds if the switch is not pushed
  }
}
/*
 * this is backward because it is tremendously more easy to test for a pin moved to ground, than one moved toward HIGH
 * a value that changes based on the power the board need, 5V, 3.3V, 9V, 12V are common values in our modern world
 * == is a test for equality, not to be confused with = "to set equal to"
 * other tests are > >= < <= != (not equal) or ! "not", a common use for ! is if you have something that must be done periodically, that is toggled between two states
 * {} marks the code to execute if (condition(s)) is/are met
 */

/*
 *  another way of looking at the same solution with multiple switches to check would be
 *  change checkSwitch to be:
 */ 

 int checkSwitch(int pinNum)
{ 
  int switchValue = digitalRead(pinNum); // make a temporary variable of type int(eger) which is the value read from an arbitrary digital input pin
  return switchValue;                       // and return that value to the caller
}
