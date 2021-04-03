#ifndef __FLASHLUX_LIGHT_STREAM_H__
#define __FLASHLUX_LIGHT_STREAM_H__

#include <Stream.h>
#include <inttypes.h>

#define LIGHT_STREAM_BUFFER_SIZE 8

class LightStream : public Stream {
  protected:
    char rxBuffer[LIGHT_STREAM_BUFFER_SIZE];
    uint8_t rxBufferHead = 0;
    uint8_t rxBufferTail = 0;
    
    bool bufferOverflow;

  public:
    LightStream();

    void receiveByte(unsigned char b);

    void begin();
    void end();
    
    int available();  // returns the number of bytes available in buffer
    int peek();       // reveals next byte in buffer without consuming
    int read();       // returns next byte in the buffer (consumes)

    // The following functions do nothing, but must be declared
    void flush() {}
    size_t write(uint8_t b) {}
};

#endif // __FLASHLUX_LIGHT_STREAM_H__
