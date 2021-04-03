
int ledPin = 13;
int switchPin = 2;

void setup() 
{
  pinMode(ledPin, OUTPUT); 
  pinMode(switchPin, INPUT_PULLUP); 
}

int checkSwitch()
{ 
  int switchValue = digitalRead(switchPin);
  return switchValue;
}


void loop()
{
  digitalWrite(ledPin, HIGH);
  delay(300);
  digitalWrite(ledPin, LOW);
  delay(1000);
  bool switchPressed = checkSwitch(); // use checkSwitch(pinNumber); to use the variant
  if (switchPressed == true)
  {
    delay(3000);
  }
}


 int checkSwitch(int pinNum)
{ 
  int switchValue = digitalRead(pinNum);
  return switchValue;
}
