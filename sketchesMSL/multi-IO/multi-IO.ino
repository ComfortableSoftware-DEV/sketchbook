void setup() 
{
  // put your setup code here, to run once:
  pinMode(3, INPUT_PULLUP); // switch to ground
  pinMode(4, OUTPUT); // switch LED
  pinMode(5, OUTPUT); // analog input led 1
  pinMode(6, OUTPUT); // analog input led 2
  pinMode(7, OUTPUT); // analog input led 3
  pinMode(8, OUTPUT); // analog input led 4
  // analog pins need no initialization unless they will be used as digital pins
  // A0 goes to a photo detector which varies between +5v and ground/-
}

int analogReadVar;
int numberLED2Light;
boolean digitalReadVar;

void loop() 
{
  // put your main code here, to run repeatedly:
  analogReadVar = analogRead(A0);
  if (analogReadVar >= 100 && analogReadVar < 300)
  {
    numberLED2Light = 1;
  }
  if (analogReadVar >= 300 && analogReadVar < 500)
  {
    numberLED2Light = 2;
  }
  if (analogReadVar >= 500 && analogReadVar < 700)
  {
    numberLED2Light = 3;
  }
  if (analogReadVar > 700)
  {
    numberLED2Light = 4;
  }
  digitalReadVar = digitalRead(3);
  if (digitalReadVar)
  {
    digitalWrite(4, HIGH);
  }
  else
  {
    digitalWrite(4, LOW);
  }
  for (int indexVar = 1; indexVar < 5; indexVar++)
  {
    if (numberLED2Light >= indexVar)
    {
      digitalWrite(4 + indexVar, HIGH);
    }
    else
    {
      digitalWrite(4 + indexVar, LOW);
    }
  }
}
