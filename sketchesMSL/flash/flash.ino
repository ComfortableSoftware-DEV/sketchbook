void setup() 
{
  // put your setup code here, to run once:
  pinMode(PB3, OUTPUT);
  digitalWrite(PB3, LOW);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  digitalWrite(PB3, HIGH);
  delay(8);
  digitalWrite(PB3, LOW);
  delay(8);
}
