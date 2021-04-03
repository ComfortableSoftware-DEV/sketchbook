#define PIN2FLASH PB9

void setup() 
{
  // put your setup code here, to run once:
  pinMode(PIN2FLASH, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  digitalWrite(PIN2FLASH, HIGH);
  delay(500);
  digitalWrite(PIN2FLASH, LOW);
  delay(1000);
}
