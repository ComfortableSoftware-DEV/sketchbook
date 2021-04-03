#ifndef __FLASHLUX_HANDSHAKE_H__
#define __FLASHLUX_HANDSHAKE_H__

#include "settings.h"
#include "light_signal.h"

/**
 * The handshake state moves from a logarithmic (geometrically decreasing)
 * phase to a linear phase, then on to a successful finish (and presumably,
 * on to get lots of data for flashing to the chip).
 * 
 * Logarithmic Phase:
 * - like a funnel that lets us sync up with the signal edge
 * - dynamically establishes lux threshold for black/white
 * 
 * Linear Phase:
 * - quick, only 8 bits
 * - one last chance to adjust our signal edge, if needed
 * 
 * Success Phase:
 * - we're done, let some other part of the 
 * 
 */
enum HandshakeState { HS_LOGARITHMIC, HS_LINEAR, HS_SUCCESS };

/**
  * To establish a one-way "handshake", we start with a large window of
  * HANDSHAKE_START_SIZE, which denotes the number of "frames" (i.e. screen
  * animation frames) that are held within the first signal state. Then
  * the signal will transition to half as many, then half again, etc. in a
  * logarithmic series down to size 1.
  *
  * 64 : 1 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * 32 : 0 --------------------------------
  * 16 : 1 ++++++++++++++++
  * 8  : 0 --------
  * 4  : 1 ++++
  * 2  : 0 --
  * 1  : 1 +
  *
  */
#define HANDSHAKE_START_SIZE 64 // number of white frames the flasher will start with
#define HANDSHAKE_HALVINGS 6 // number of halvings before we get to 1, i.e. log2(64)

// Number of samples to track in time
#define LUX_SAMPLE_SIZE 7

// Number of microseconds between samples
#define LUX_SAMPLE_DELAY 200000

// Minimum delta between min lux and max lux
#define LUX_MINIMUM_DELTA 20


class Handshake {
  protected:
    void syncStepLogarithmic(LightSignal* lightsig);
    void syncStepLinear(LightSignal* lightsig);

  public:
    int currentStep;
    int previousStep;
    char currentColorState;
    char previousColorState;
    enum HandshakeState state;
  
    unsigned long lastLuxObservationTime;
    unsigned long aTransitionedAt;
    unsigned long bTransitionedAt;

    /**
     * Track lux values over time so we can create a dynamic "iris"
     * that adapts to various lighting conditions. The goal here is
     * to get a threshold that distinguishes between "white" and
     * "black".
     */
    unsigned int luxT[LUX_SAMPLE_SIZE];

    Handshake(void);
    void reset(void);
    bool isEstablished(void);
    void establishStep(LightSignal* lightsig);
    unsigned int observeLuxAndGetThreshold(unsigned int lux);
    int findClosestStepForPulseWidth(unsigned int pulseWidth);
};

#endif // __FLASHLUX_HANDSHAKE_H__
