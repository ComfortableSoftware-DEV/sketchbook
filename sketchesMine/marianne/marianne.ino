void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

long int MIC;

void loop() 
{
  // put your main code here, to run repeatedly:
  MIC = analogRead(A0);
  Serial.println(MIC);
}
