
// include the no-hardware-added capacitive touch library
#include <ADCTouch.h>


// and the simple "player piano" app that will be our first synthesizer
#include <MusicWithoutDelay.h>
// quick reference for this  library at
// https://github.com/nathanRamaNoodles/Noodle-Synth#simple-continuous-frequency


// pins for output are 11 (channel A) , 3 (channel B) to ground for NPN or +5 for PNP transistor amplification
//write 6 "songs" of one note each for speed and bonus mojo
//const char song[]  // make a neighborhood/set of notes
// PROGMEM =  // store it in program memory, hard memory, not RAM
// name:options:notes

const char note1[] PROGMEM = "C Scale:d=4:c,d,e,f,g,a,b,c1";   //plays c scale,  You can add more notes to get creative. //d=4 means that every note without a number in front of the letter is assumed to be a quarter note.
const char note2[] PROGMEM = ":d=4:e,f,g,a,b,c1,d1,e1";   //plays c scale starting at e
const char song01[] PROGMEM = "::a";  // plays a
const char song02[] PROGMEM = "::b";  // plays b

//const char song03[] PROGMEM = "::c";  // plays c
//const char song04[] PROGMEM = "::d";  // plays d

//start 6 instruments with those songs
MusicWithoutDelay instrument01(song01);
MusicWithoutDelay instrument02(song02);
MusicWithoutDelay instrument03(note1);
MusicWithoutDelay instrument04(note2);

//MusicWithoutDelay instrument05(song03);
//MusicWithoutDelay instrument06(song04);

int threshold = 40; // a wild guess at the point where a "key" is touched more later

//reference values for "untouched" this is giving them a place to live, an address as it were
int ref01, ref02, ref03, ref04;

// this code will only run at boot/reset, it is used to initialize things to known good values, open devices, etc.
void setup() 
{
  instrument01.begin(CHB, TRIANGLE, ENVELOPE0, 0);
  instrument02.begin(TRIANGLE, ENVELOPE0, 0);
  instrument03.begin(TRIANGLE, ENVELOPE0, 0);
  instrument04.begin(TRIANGLE, ENVELOPE0, 0);
  //instrument05.begin(TRIANGLE, ENVELOPE0, 0);
  //instrument06.begin(TRIANGLE, ENVELOPE0, 0);

    // start the serial terminal we will be writing information to as we "tune" our primitive instrument
    Serial.begin(9600);

    // reference values "untouched"
    // they will likely be different on each 'pad'
    // 800 means take 800 samples over a short period of time
    ref01 = ADCTouch.read(A1, 800);
    ref02 = ADCTouch.read(A2, 800);
    ref03 = ADCTouch.read(A3, 800);
    ref04 = ADCTouch.read(A4, 800);
    //ref05 = ADCTouch.read(A5, 500);
    //ref06 = ADCTouch.read(A6, 500);
} 


// this code will run constantly after the setup procedure has finished
void loop() 
{
    // read all of our "keys"
    // without the second parameter these will take 100 samples each
    // by the built in default
    int value01 = ADCTouch.read(A0);
    int value02 = ADCTouch.read(A1);
    int value03 = ADCTouch.read(A2);
    int value04 = ADCTouch.read(A3);
    //int value05 = ADCTouch.read(A4);
    //int value06 = ADCTouch.read(A5);

    instrument01.update();
    instrument02.update();
    instrument03.update();
    instrument04.update();
    //instrument05.update();
    //instrument06.update();

    // now we subtract the numbers we got when we took 800 samples 
    value01 -= ref01;
    value02 -= ref02;
    value03 -= ref03;
    value04 -= ref04;
    //value05 -= ref05;
    //value06 -= ref06;

    // now we print the values of each note and over a WAG (wildly aggressive guess) at the 
    // difference between pressed and not pressed values
    // 40 is that guess in a relatively dry house, set earlier (line 35)
    
    Serial.print("01 ");
    Serial.print(value01 > threshold);  // send true/false if value went over our threshold or not to terminal
    Serial.print("\t");  // send a tab character to the terminal
    Serial.print(value01);  // now send the actual value so we can see what needs to change

    Serial.print("\t02 ");
    Serial.print(value02 > threshold);  // send true/false if value went over our threshold to terminal
    Serial.print("\t");  // send a tab character to the terminal
    Serial.print(value02);  // now send the actual value so we can see what needs to change
    
    Serial.print("\t03 ");
    Serial.print(value03 > threshold);  // send true/false if value went over our threshold to terminal
    Serial.print("\t");  // send a tab character to the terminal
    Serial.print(value03);  // now send the actual value so we can see what needs to change
    
    Serial.print("\t04 ");
    Serial.print(value04 > threshold);  // send true/false if value went over our threshold to terminal
    Serial.print("\t");  // send a tab character to the terminal
    Serial.println(value04);  // now send the actual value so we can see what needs to change
    
    //Serial.print("\t05 ");
    //Serial.print(value05 > threshold);  // send true/false if value went over our threshold to terminal
    //Serial.print("\t");  // send a tab character to the terminal
    //Serial.println(value05);  // now send the actual value so we can see what needs to change
    
    //Serial.print("\t06 ");
    //Serial.print(value06 > threshold);  // send true/false if value went over our threshold to terminal
    //Serial.print("\t");  // send a tab character to the terminal
    //Serial.println(value06);  // now send the actual value so we can see what needs to change

    // now we go through the keysand play a note if the key is pressed/touched
    if (value01 > threshold)
    {
      instrument01.play(1);
    }

    if (value02 > threshold)
    {
      instrument02.play(1);
    }
    
    if (value03 > threshold)
    {
      instrument03.play(1);
    }
    
    if (value04 > threshold)
    {
      instrument04.play(1);
    }
    
    //if (value05 > 40)
    //{
      //instrument05.play(1);
    //}
    
    //if (value06 > 40)
    //{
      //instrument06.play(1);
    //}

    //and make sure we update each note so they play through
    instrument01.update();
    instrument02.update();
    instrument03.update();
    instrument04.update();
    //instrument05.update();
    //instrument06.update();
    
}   
