#include <Arduino.h>  // Defines micros()
#include "light_signal.h"

/** 
 * If `signalEdge` is more than one `SIGNAL_PERIOD` behind, catch up to `currentTime`.
 * 
 * Example:
 *   signalEdge: 4
 *   currentTime: 51
 *   SIGNAL_PERIOD: 10
 *   
 *   In this case, the new `signalEdge` should be 44 (true to the `signalEdge`
 *   cycle start time, less than `currentTime`, but no less than `currentTime - 10`).
 */
void LightSignal::signalEdgeCatchUp(void) {
  long delta = this->currentTime - this->signalEdge;    // e.g. 51 - 4 = 47
  if (delta > SIGNAL_PERIOD) {                          // e.g. 47 > 10
    long periods = delta / SIGNAL_PERIOD;               // e.g. 47 / 10 = 4
    this->signalEdge += periods * SIGNAL_PERIOD;        // e.g. add (4 * 10), so signalEdge now = 44
  }
}

/**
 * Initialize the LightSignal instance variables.
 */
LightSignal::LightSignal() {
  this->currentTime = 0;
  this->signalEdge = 0;
  this->lux = 0;
  this->threshold = 150;
}

bool LightSignal::isHigh(void) {
  return this->lux >= this->threshold;
}

bool LightSignal::isLow(void) {
  return this->lux < this->threshold;
}

unsigned char LightSignal::getBit(void) {
  return (this->lux >= this->threshold ? 1 : 0);
}

void LightSignal::setLux(unsigned int _lux) {
  this->lux = _lux;
}

void LightSignal::setThreshold(unsigned int _threshold) {
  this->threshold = _threshold;
}

void LightSignal::signalEdgeWait(void) {
  // Doesn't hurt to catch up if needed
  this->signalEdgeCatchUp();
  
  // Wait for signal edge
  while (this->currentTime < this->signalEdge + SIGNAL_PERIOD) {
    this->currentTime = micros();
  }
}

void LightSignal::signalEdgeAdvance(void) {
  this->signalEdge += SIGNAL_PERIOD;
}

void LightSignal::signalEdgeAdjust(unsigned long adjustment) {
  this->signalEdge -= adjustment;
}

void LightSignal::signalEdgeNudge(void) {
  this->signalEdge -= SIGNAL_EDGE_NUDGE;
}

