// no libraries needed for this project

//this program and the accompanying circuit is designed around a 12vdc valve that is powered to close
//to change to using a 12vdc valve that is powered to open exchange HIGH for LOW and LOW for HIGH for pin 3

//start a function which will be run only the first time the arduino is powered/reset/etc.
void setup() 
{
  // put your setup code here, to run once:
  //set pins 2 and 3 on the digital side of the uno to be outputs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  // set pin 2 to low, this will be used to supply power to the moisture detector only when we want to check the moisture
  digitalWrite(2, LOW);
  // set pin 3 to high, this will tell the transistor to close the valve
  digitalWrite(3, HIGH); // this would be LOW with a powered to open valve
}

void loop() 
{
  // put your main code here, to run repeatedly:
  // set pin 2 to high so we can measure the moisture level
  digitalWrite(2, HIGH);
  //create an integer variable called moisture and read the value of analog port 0 into that variable
  int moisture = analogRead(A0);
  // if the value from analog port 0 is less than 50 open the valve
  if (moisture < 50)
  {
    // way too dry, if we had multiple valves or a variable valve we would 
    // open all valves or a variable valve all the way
    openValve();
  }
  // if the soil is just a little dry this will open the valve just a little or only one if multiple are used
  if (moisture >50 && moisture <500)
  {
    openValve();
  }
  //if the soil is as moist as we want it to be close the valve(s)
  if (moisture > 500)
  {
    closeValve();
  }
  // stop sending power to the moisture probe
  digitalWrite(2, LOW);
  // sleep for 30 minutes before checking the moisture again
  // this time is in miliseconds, 1 second is 1000 miliseconds, 60 seconds * 30 minutes is 1800
  // times 1000 is 1800000
  delay(1800000);
}

// make a function that returns nothing (void) to open the valve
void openValve()
{
  // by setting pin 3 to low, removing power to the valve
  digitalWrite(3, LOW); // this would be HIGH in a powered to open valve
}

// a function to close the valve opposite of open the valve
void closeValve()
{
  digitalWrite(3, HIGH); // this would be LOW for a powered to open valve
}

