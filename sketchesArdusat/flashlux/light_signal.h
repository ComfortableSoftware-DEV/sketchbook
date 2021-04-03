#ifndef __FLASHLUX_LIGHT_SIGNAL_H__
#define __FLASHLUX_LIGHT_SIGNAL_H__

/**
  * Most monitors can display 60 frames per second, so let's
  * choose a "cycles per second" value that falls within this
  * expectation.
  */
#define SIGNAL_HZ 30

/**
  * Period (1.0/Frequency) in microseconds is 1,000,000/30 = 33,333.33
  */
#define SIGNAL_PERIOD 33333

/**
 * An amount of time to nudge (adjust) the signal edge, in microseconds.
 */
#define SIGNAL_EDGE_NUDGE (SIGNAL_PERIOD / 15)

class LightSignal {
  protected:
    void signalEdgeCatchUp(void);

  public:
    unsigned long currentTime = 0;
    unsigned long signalEdge = 0;
    unsigned int lux;
    unsigned int threshold;

    LightSignal(void);

    bool isHigh(void);
    bool isLow(void);
    unsigned char getBit(void);

    // Lux
    void setLux(unsigned int _lux);
    void setThreshold(unsigned int _threshold);

    // Signal Edge
    void signalEdgeWait(void);
    void signalEdgeAdvance(void);
    void signalEdgeAdjust(unsigned long adjustment);
    void signalEdgeNudge(void);
};

#endif // __FLASHLUX_LIGHT_SIGNAL_H__
