// change up and down the echo cells
// switch 2nd oscillator in and out
// pick a table for each oscillator (8)
// pick a number of steps difference 0-12 in many small increments

#include <math.h>
#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_rand.h>
#include <tables/cos2048_int8.h>
#include <tables/cosphase2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/sin2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/triangle2048_int8.h>
#include <tables/triangle_dist_cubed_2048_int8.h>
#include <tables/triangle_dist_squared_2048_int8.h>
#include <tables/triangle_hermes_2048_int8.h>
#include <tables/triangle_valve_2048_int8.h>
#include <tables/triangle_valve_2_2048_int8.h>
#include <RollingAverage.h>
#include <ControlDelay.h>

#include <Adafruit_NeoPixel.h>

#define NUM_TABLES 11
#define ONESTEP  1.059463
byte myTable1, myTable2;
byte NUMSTEPS = 0;

// original
unsigned int echo_cells_1 = 32;
unsigned int echo_cells_2 = 60;
unsigned int echo_cells_3 = 127;
// playing around all over the place
//unsigned int echo_cells_1 = 5;
//unsigned int echo_cells_2 = 50;
//unsigned int echo_cells_3 = 75;

#define INPUT_PIN PA0
#define CONTROL_RATE 64
#define LEDDATA 7
#define NUMLED 15
#define LIGHTRATE 2
int LEDINDEX, LIGHTINDEX = 0;
bool STOPVALUES[8] = 
{
  false, // steps difference between osc1 and osc2
  false, // oscil2 on/off
  false, // oscil1 wave 01
  false, // oscil1 wave 10
  false, // oscil2 wave 01
  false, // oscil2 wave 10
  false,
  false // active / silent
};
byte waveSelected[8] =
{
  0, 0, 0, 0,
  0, 0, 0, 0
};

const int STOPPINS[8] =
{
  2,
  3,
  4,
  5,
  7,
  8,
  10,
  11
};

ControlDelay <128, int> kDelay; // 2seconds

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
// oscils to compare bumpy to averaged control input
Oscil<2048, AUDIO_RATE > oscil1_1;
Oscil<2048, AUDIO_RATE > oscil1_2;
Oscil<2048, AUDIO_RATE > oscil1_3;
Oscil<2048, AUDIO_RATE > oscil1_4;

Oscil<2048, AUDIO_RATE > oscil2_1;
Oscil<2048, AUDIO_RATE > oscil2_2;
Oscil<2048, AUDIO_RATE > oscil2_3;
Oscil<2048, AUDIO_RATE > oscil2_4;

const int8_t * tables[NUM_TABLES] =
{
  SIN2048_DATA,
  SAW2048_DATA,
  TRIANGLE2048_DATA,
  SQUARE_NO_ALIAS_2048_DATA,
  COS2048_DATA,
  COSPHASE2048_DATA,
  TRIANGLE_DIST_CUBED_2048_DATA,
  TRIANGLE_DIST_SQUARED_2048_DATA,
  TRIANGLE_HERMES_2048_DATA,
  TRIANGLE_VALVE_2048_DATA,
  TRIANGLE_VALVE_2_2048_DATA
};

// use: RollingAverage <number_type, how_many_to_average> myThing
RollingAverage <int, 32> kAverage; // how_many_to_average has to be power of 2
int averaged;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLED, LEDDATA, NEO_GRB + NEO_KHZ800);

void setup()
{
  randSeed();
  kDelay.set(echo_cells_1);
  strip.begin();
  blankStrip();
  startMozzi();
  setOscils();
  //Serial.begin(115200);
  pinMode(STOPPINS[0], INPUT_PULLUP);
  pinMode(STOPPINS[1], INPUT_PULLUP);
  pinMode(STOPPINS[2], INPUT_PULLUP);
  pinMode(STOPPINS[3], INPUT_PULLUP);
  pinMode(STOPPINS[4], INPUT_PULLUP);
  pinMode(STOPPINS[5], INPUT_PULLUP);
  pinMode(STOPPINS[6], INPUT_PULLUP);
  pinMode(STOPPINS[7], INPUT_PULLUP);

}

void setOscils()
{
  kDelay.set(echo_cells_1);
  oscil1_1.setTable(tables[waveSelected[(0 * 4) + 0]]);
  oscil1_2.setTable(tables[waveSelected[(0 * 4) + 1]]);
  oscil1_3.setTable(tables[waveSelected[(0 * 4) + 2]]);
  oscil1_4.setTable(tables[waveSelected[(0 * 4) + 3]]);

  oscil2_1.setTable(tables[waveSelected[(1 * 4) + 0]]);
  oscil2_2.setTable(tables[waveSelected[(1 * 4) + 1]]);
  oscil2_3.setTable(tables[waveSelected[(1 * 4) + 2]]);
  oscil2_4.setTable(tables[waveSelected[(1 * 4) + 3]]);
}

void updateControl()
{
  checkStops();
  if (STOPVALUES[7])
  {
    blankStrip();
    return;
  }
  int bumpy_input = mozziAnalogRead(INPUT_PIN);
  averaged = kAverage.next(bumpy_input);
  //Serial.println(bumpy_input);
  //Serial.println(averaged);
  //Serial.println("");
  fillStrip(averaged);
  oscil1_1.setFreq(averaged);
  oscil1_2.setFreq(kDelay.next(averaged));
  oscil1_3.setFreq(kDelay.read(echo_cells_2));
  oscil1_4.setFreq(kDelay.read(echo_cells_3));
  int FREQDIFF = (int) ((float)averaged * (float)pow(ONESTEP, NUMSTEPS) - 1); // switch this in and out to have a difference between the two oscillators
  oscil2_1.setFreq(averaged - FREQDIFF);
  oscil2_2.setFreq(kDelay.next(averaged) - FREQDIFF);
  oscil2_3.setFreq(kDelay.read(echo_cells_2) - FREQDIFF);
  oscil2_4.setFreq(kDelay.read(echo_cells_3) - FREQDIFF);
}

void fillStrip(int SEED)
{
  LIGHTINDEX++;
  if (LIGHTINDEX < LIGHTRATE)
  {
    return;
  }
  LIGHTINDEX = 0;
  int REDLED, GREENLED, BLUELED;
  // blue for low frequency
  REDLED = rand(0, 64);
  GREENLED = rand(0, 128);
  BLUELED = rand(220, 255);
  // green middle frequencies
  if (SEED > 450)
  {
    REDLED = rand(0, 64);
    GREENLED = rand(220, 255);
    BLUELED = rand(0, 64);
  }
  // red high frequencies
  if (SEED > 600)
  {
    REDLED = rand(220, 255);
    GREENLED = rand(0, 96);
    BLUELED = rand(64, 96);
  }
   
  strip.setPixelColor(LEDINDEX, strip.Color(REDLED + rand(-5, 5), GREENLED + rand(-4, 4), BLUELED + rand(-7, 7)));
  LEDINDEX++;
  if (LEDINDEX == NUMLED)
  {
    LEDINDEX = 0;
  }
  strip.show();
}

int updateAudio()
{
  if (STOPVALUES[7])
  {
    return 0;
  }
  int ret1, ret2;
  ret1 = 3 * ((int)oscil1_1.next() + oscil1_2.next() + (oscil1_3.next() >> 1) + (oscil1_4.next() >> 2)) >> 3;
  if (!STOPVALUES[1])
  {
    ret2 = 3 * ((int)oscil2_1.next() + oscil2_2.next() + (oscil2_3.next() >> 1) + (oscil2_4.next() >> 2)) >> 3;
  }
  else
  {
    ret2 = 0;
  }
  return ret1 | ret2;
}

void blankStrip()
{
  for (int TI1 = 0; TI1 < NUMLED; TI1++)
  {
    strip.setPixelColor(TI1, 0);
  }
  strip.show();
}

void setWaveSelected()
{
  for (byte TBt1 = 0; TBt1 < 4; TBt1++)
  {
    waveSelected[TBt1] = (2 * STOPVALUES[3]) + STOPVALUES[2];
  }
  for (byte TBt1 = 4; TBt1 < 8; TBt1++)
  {
    waveSelected[TBt1] = (2 * STOPVALUES[5]) + STOPVALUES[4];
  }
  //for (byte TBt1 = 0; TBt1 < 8; TBt1++)
  //{
    //Serial.print(STOPVALUES[TBt1]);
  //}
  //Serial.println("");
  //for (byte TBt1 = 0; TBt1 < 8; TBt1++)
  //{
    //Serial.print(waveSelected[TBt1]);
    //Serial.print(", ");
  //}
  //Serial.println("");
}

void checkStops()
{
  bool TB1;
  byte TBt1;
  for (TBt1 = 0; TBt1 < 8; TBt1++)
  {
    TB1 = digitalRead(STOPPINS[TBt1]);
    if (TB1 != STOPVALUES[TBt1])
    {
      STOPVALUES[TBt1] = TB1;
      if (TBt1 == 0)
      {
        if (TB1)
        {
          NUMSTEPS = 0;
        }
        else
        {
          NUMSTEPS = 5;
        }
      }
      if (TBt1 >= 2 && TBt1 <= 5)
      {
        setWaveSelected();
        setOscils();
      }
    }
  }
}

void loop()
{
  audioHook();
}
