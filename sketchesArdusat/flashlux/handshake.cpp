#include <WString.h>  // defines F() macro and includes `memset` from string.h
#include "handshake.h"
#include "debug.h"

Handshake::Handshake(void) {
  this->reset();

  this->currentColorState = 0;
  this->previousColorState = 0;
}

void Handshake::reset() {
  this->currentStep = -1;
  this->previousStep = -1;

  // Start over with the declining logarithmic algorithm
  this->state = HS_LOGARITHMIC;

  this->lastLuxObservationTime = 0;

  memset(this->luxT, 0, sizeof(this->luxT));

  DEBUGPLN(F("Handshake::reset"));
}

bool Handshake::isEstablished(void) {
  return (this->state == HS_SUCCESS);
}

/**
 * Keep a running history of lux observations, so we can get dynamic min/max values,
 * and produce a dynamic light threshold value. The threshold differentiates white
 * from black, or 1s from 0s.
 */
unsigned int Handshake::observeLuxAndGetThreshold(unsigned int lux) {
  // Set ridiculously high min and low max
  unsigned int luxMin = 100000, luxMax = 0;

  // Set the last historical lux value to current lux value
  this->luxT[LUX_SAMPLE_SIZE-1] = lux;
  
  for (int i = 0; i < LUX_SAMPLE_SIZE; i++) {
    // Shuffle record of lux sampling values over by 1
    if (i < LUX_SAMPLE_SIZE-1) this->luxT[i] = this->luxT[i+1];
    
    if (luxMin > this->luxT[i]) luxMin = this->luxT[i];
    if (luxMax < this->luxT[i]) luxMax = this->luxT[i];
  }
  if (luxMin > luxMax - LUX_MINIMUM_DELTA) {
    luxMin = luxMax - LUX_MINIMUM_DELTA;
  }

  // Return the average of min and max, which is a reasonable lux threshold
  return (luxMin + luxMax) / 2;
}

/**
 * Take one step towards establishing a signal edge and dynamic luminosity threshold.
 * This involves both the initial logarithmic sync as well as the flickering sync in
 * turn.
 */
void Handshake::establishStep(LightSignal* lightsig) {
  switch(this->state) {
  case HS_LOGARITHMIC:
    this->syncStepLogarithmic(lightsig);
    break;
  case HS_LINEAR:
    this->syncStepLinear(lightsig);
    break;
  case HS_SUCCESS:
    // do nothing? warning?
    break;
  }
}

/** 
 *  Establish timing sync through logarithmic handshake protocol 
 */
void Handshake::syncStepLogarithmic(LightSignal* lightsig) {
  unsigned int pulseWidth;

  if (lightsig->currentTime > this->lastLuxObservationTime + LUX_SAMPLE_DELAY) {
    this->lastLuxObservationTime = lightsig->currentTime;
    lightsig->setThreshold(this->observeLuxAndGetThreshold(lightsig->lux));
  }
  
//    DEBUGP(F("lux: "));
//    DEBUGP(lux);
//    DEBUGP(F(", threshold: "));
//    DEBUGPLN(lightsig->threshold);

  // Interpret the light value as either a 0 or a 1
  if (lightsig->isHigh()) {
    this->currentColorState = 1;
    setLED(LED_ON);
  } else {
    this->currentColorState = 0;
    setLED(LED_OFF);
  }

  // During the handshake, whenever we cross a boundary from 0 to 1
  // or from 1 to 0, it's an important marker that we can use to
  // establish the signal edge [https://en.wikipedia.org/wiki/Signal_edge]
  if (this->previousColorState != this->currentColorState) {
    this->aTransitionedAt = this->bTransitionedAt;
    this->bTransitionedAt = lightsig->currentTime;

  DEBUGP(F("bit: "));
  DEBUGPLN(this->previousColorState, DEC);

    /**
     * Add 10 microseconds to the delta before dividing to assure that integer
     * division doesn't round down due to a very small sampling error.
     */
    pulseWidth = (this->bTransitionedAt - this->aTransitionedAt + 10) / SIGNAL_PERIOD;

//    DEBUGP(F("time1: "));
//    DEBUGP(this->aTransitionedAt);
//    DEBUGP(F(", time2: "));
//    DEBUGP(this->bTransitionedAt);
//    DEBUGP(F("delta: "));
//    DEBUGP(this->bTransitionedAt - this->aTransitionedAt);
//    DEBUGP(F(", signalEdge: "));
//    DEBUGP(lightsig->signalEdge);
//    DEBUGP(F(", pulseWidth: "));
//    DEBUGPLN(pulseWidth);

    this->currentStep = findClosestStepForPulseWidth(pulseWidth);

//    DEBUGP(F("currentStep: "));
//    DEBUGP(this->currentStep);
//    DEBUGP(F(", previousStep: "));
//    DEBUGPLN(this->previousStep);

    if (this->currentStep == HANDSHAKE_HALVINGS) {
      // Initial pulse means we're starting the handshake
      DEBUGPLN(F("successful handshake step 6 (init)"));
    } else if (this->currentStep == HANDSHAKE_HALVINGS-1
            && this->previousStep != HANDSHAKE_HALVINGS) {
      // This may be an initial pulse, but we may be off by one frame, so adjust
      DEBUGPLN(F("successful handshake step 5 (?)"));
      lightsig->signalEdgeNudge();
      this->currentStep += 1;
      DEBUGP(F("step adjusted to: "));
      DEBUGPLN(this->currentStep);
    } else if (this->currentStep == 0 && this->previousStep == 1) {
      // We're done! We've accomplished the logarithmic handshake step.
      this->currentStep = 0;
      this->state = HS_LINEAR;
      DEBUGPLN(F("logarithmic sync complete"));
    } else if (this->currentStep == this->previousStep - 1 && this->previousStep >= 1) {
      // Good, we're on course for a handshake
      DEBUGP(F("successful handshake step "));
      DEBUGPLN(this->currentStep);
    } else if (this->currentStep == this->previousStep - 2 && this->previousStep >= 2) {
      DEBUGP(F("successful handshake step "));
      DEBUGP(this->currentStep);
      DEBUGPLN(F(" (?)"));
      // Off-by-one, attempt to adjust signalEdge
      lightsig->signalEdgeNudge();
      this->currentStep += 1;
      DEBUGP(F("step adjusted to: "));
      DEBUGPLN(this->currentStep);
    } else {
      // Logarithmic handshake out of sequence, start the handshake process over
      DEBUGPLN(F("log sync has step out of seq"));
      this->reset();
    }

    this->previousStep = this->currentStep;
    this->previousColorState = this->currentColorState;
  }
}

/** 
 *  Establish timing sync through linear handshake protocol 
 */
void Handshake::syncStepLinear(LightSignal* lightsig) {
  char nowBit;

  nowBit = lightsig->getBit();

  DEBUGP(F("linear bit: "));
  DEBUGPLN(nowBit, DEC);

  if (nowBit == this->currentStep % 2) {
    if (currentStep < 6) {
      // hmm, not what we expected, try nudging the signal edge
      lightsig->signalEdgeNudge();
      DEBUGPLN(F("value is off, nudging "));
    } else {
      // if we have an error after the 6th bit, we call it quits
      DEBUGPLN(F("linear sync has too many steps"));
      this->reset();
    }
  } else {
    // great! this is what we expected
  }

  this->currentStep++;

  // Note: Due to the way we hand off the Logarithmic sync to the Linear sync,
  // we actually lose 1 bit in between. So we check for currentStep == 7 instead
  // of 8 here.
  if (currentStep == 7) {
    // success!
    this->state = HS_SUCCESS;
  }
 
}

/**
 * Using microsecond timestamps, time1 and time2, determine which handshake
 * "band" we're in. Start at the largest band (i.e. 64  frames) and move down
 * by half for each iteration. Return the best match:
 * 
 *  band 0 = 1 frame
 *  band 1 = 2 frames
 *  band 2 = 4 frames
 *  band 3 = 8 frames
 *  band 4 = 16 frames
 *  band 5 = 32 frames
 *  band 6 = 64 frames
 *  
 */
int Handshake::findClosestStepForPulseWidth(unsigned int pulseWidth) {
  unsigned int expectedPulseWidth = 1<<(HANDSHAKE_HALVINGS);   // start at 64 (= 1<<6)
  int step;

  for (step = HANDSHAKE_HALVINGS; step >= 0; step--) {
    if (pulseWidth >= expectedPulseWidth) {
      return step;
    }
    expectedPulseWidth >>= 1; // halve
  }
  return -1;
}

