void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
}

boolean pin2 = LOW;
boolean pin3 = HIGH;

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(2, pin2);
  digitalWrite(3, pin3);
  delay(2000);
  pin2 = !pin2;
  pin3 = !pin3;
  
}
