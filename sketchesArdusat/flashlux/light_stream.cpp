#include "light_stream.h"

LightStream::LightStream() {
  this->bufferOverflow = false;
}

void LightStream::receiveByte(unsigned char b) {
  if ((this->rxBufferTail + 1) % LIGHT_STREAM_BUFFER_SIZE == this->rxBufferHead) { 
    this->bufferOverflow = true; 
  } else {
    this->rxBuffer[this->rxBufferTail] = b;
    this->rxBufferTail = (this->rxBufferTail + 1) % LIGHT_STREAM_BUFFER_SIZE;
  }
}

void LightStream::begin() {}

void LightStream::end() {}

int LightStream::available() {
  if(this->bufferOverflow) {
    return -1; 
  }
  return
    (this->rxBufferTail + LIGHT_STREAM_BUFFER_SIZE - this->rxBufferHead)
    % LIGHT_STREAM_BUFFER_SIZE;
}

int LightStream::peek() {
  if (this->rxBufferHead == this->rxBufferTail) {
    return -1;
  }
  return this->rxBuffer[this->rxBufferHead];
}

int LightStream::read() {
  // buffer will have room after we read
  this->bufferOverflow = false;
  
  if (this->rxBufferHead == this->rxBufferTail) {
    return -1;
  }
  uint8_t nextChar = this->rxBuffer[this->rxBufferHead];
  
  // increment the buffer head pointer
  this->rxBufferHead = (this->rxBufferHead + 1) % LIGHT_STREAM_BUFFER_SIZE;
  
  return nextChar;
}


