int firstSensor = 0;    // first analog sensor
int secondSensor = 0;   // second analog sensor
int thirdSensor = 0;    // digital sensor
int inByte = 0;         // incoming serial byte

void setup() {
  pinMode(2, INPUT);   // digital sensor is on digital pin 2
  pinMode(13, OUTPUT);
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:
//  if (Serial.available() > 0) {
    // get incoming byte:
    //inByte = Serial.read();
    // read first analog input:
    firstSensor = analogRead(A0);
    // read second analog input:
    secondSensor = analogRead(A1);
    // read  switch, map it to 0 or 255L
    //thirdSensor = map(digitalRead(2), 0, 1, 0, 255);
    // send sensor values:
//    if (firstSensor < 1020)
//    {
      Serial.print("array ");
      Serial.println(firstSensor);
      //Serial.print(" level ");
      //Serial.println(secondSensor);
      //delay(1000);
//    }
    //Serial.print(",");
    //Serial.print(secondSensor);
    //Serial.print(",");
    //Serial.println(thirdSensor);
  //}
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}

