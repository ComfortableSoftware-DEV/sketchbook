#include <TTS.h>
// include the library we are using to make the arduino talk

#define PWM 10
// the GPIO pin we are going to use in P)ulse W)idth M)odulation mode to connect to a transistor

TTS text2speech(PWM);
// make an object of type TTS called text2speech and tell that object what pin we want it to use for output

void setup() 
{
  // nothing to setup, it is a good idea to have an empty function to let the arduino compiler that we really don't want anything done to initialize
}

void loop() 
{
  text2speech.setPitch(8); // possible pitch values for most boards 1, 2, 4, 6, 8, 10, 13, 16
  // use the object we defined earlier to set the pitch of the speech to one of the preset values that will be used to set the timer.
  // this value must be one that makes the internal timer run at a speed that will make an even power of two for the PWN pulse width
  // lower number is higher pitch, higher pitches will be easier to hear on the small speakers we are using
  
  text2speech.sayText("Hello  Valerie and Rajah! How are you doing?");
  // use the object to speak what is between the ""s punctuation such as ! and ? will have a minor to major effect depending on the board,
  // the uno will have a medium effect that most people can hear
  
  delay(500);
  // sleep for 1/2 second

  text2speech.setPitch(1);
  // set a new pitch just for fun
  
  text2speech.sayText("I am fine, thank you.");
  // again say what is between the "", the , and . are not heard on boards like the uno with a processor speed less than about 20mhz

  delay(1000);
  // sleep for one second
 
}  

