/*
  hamming.cpp - hamming(8,4) ECC library for Arduino.
  Copyright (c) 2016 Jose M Sirvent.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "hamming.h"
#include "debug.h"

// 256 bytes for hamming code receive lookup
static const uint8_t _rxHammingTable[] PROGMEM = {
  0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x10, 0x08, 0x00, 0x10, 0x10, 0x04, 0x10, 0x02, 0x01, 0x10,
  0x00, 0x10, 0x10, 0x03, 0x10, 0x05, 0x01, 0x10, 0x10, 0x09, 0x01, 0x10, 0x01, 0x10, 0x01, 0x01,
  0x00, 0x10, 0x10, 0x03, 0x10, 0x02, 0x06, 0x10, 0x10, 0x02, 0x0a, 0x10, 0x02, 0x02, 0x10, 0x02,
  0x10, 0x03, 0x03, 0x03, 0x0b, 0x10, 0x10, 0x03, 0x07, 0x10, 0x10, 0x03, 0x10, 0x02, 0x01, 0x10,
  0x00, 0x10, 0x10, 0x04, 0x10, 0x05, 0x06, 0x10, 0x10, 0x04, 0x04, 0x04, 0x0c, 0x10, 0x10, 0x04,
  0x10, 0x05, 0x0d, 0x10, 0x05, 0x05, 0x10, 0x05, 0x07, 0x10, 0x10, 0x04, 0x10, 0x05, 0x01, 0x10,
  0x10, 0x0e, 0x06, 0x10, 0x06, 0x10, 0x06, 0x06, 0x07, 0x10, 0x10, 0x04, 0x10, 0x02, 0x06, 0x10,
  0x07, 0x10, 0x10, 0x03, 0x10, 0x05, 0x06, 0x10, 0x07, 0x07, 0x07, 0x10, 0x07, 0x10, 0x10, 0x0f,
  0x00, 0x10, 0x10, 0x08, 0x10, 0x08, 0x08, 0x08, 0x10, 0x09, 0x0a, 0x10, 0x0c, 0x10, 0x10, 0x08,
  0x10, 0x09, 0x0d, 0x10, 0x0b, 0x10, 0x10, 0x08, 0x09, 0x09, 0x10, 0x09, 0x10, 0x09, 0x01, 0x10,
  0x10, 0x0e, 0x0a, 0x10, 0x0b, 0x10, 0x10, 0x08, 0x0a, 0x10, 0x0a, 0x0a, 0x10, 0x02, 0x0a, 0x10,
  0x0b, 0x10, 0x10, 0x03, 0x0b, 0x0b, 0x0b, 0x10, 0x10, 0x09, 0x0a, 0x10, 0x0b, 0x10, 0x10, 0x0f,
  0x10, 0x0e, 0x0d, 0x10, 0x0c, 0x10, 0x10, 0x08, 0x0c, 0x10, 0x10, 0x04, 0x0c, 0x0c, 0x0c, 0x10,
  0x0d, 0x10, 0x0d, 0x0d, 0x10, 0x05, 0x0d, 0x10, 0x10, 0x09, 0x0d, 0x10, 0x0c, 0x10, 0x10, 0x0f,
  0x0e, 0x0e, 0x10, 0x0e, 0x10, 0x0e, 0x06, 0x10, 0x10, 0x0e, 0x0a, 0x10, 0x0c, 0x10, 0x10, 0x0f,
  0x10, 0x0e, 0x0d, 0x10, 0x0b, 0x10, 0x10, 0x0f, 0x07, 0x10, 0x10, 0x0f, 0x10, 0x0f, 0x0f, 0x0f
};

Hamming::Hamming(Stream* stream) {
  clearMessage();
  m_rHigh = 0;
  m_rLow = 0;
  m_receivingMessage = false;
  m_messageReady = false;
  m_stream = stream;
}

Hamming::~Hamming() {
}

void Hamming::clearMessage() {
  memset(m_message, '\0', SIZE);
  m_messageIndex = 0;
  m_messageReady = false;
}

uint8_t Hamming::rx(uint8_t data) {
  return pgm_read_byte(_rxHammingTable + data);
}

uint8_t Hamming::isMessageReady() {
  static uint8_t counter = 0;
  
  while(m_stream->available()) {
    uint8_t c = m_stream->read();
    DEBUGP(F("read hamm byte: "));
    DEBUGPLN(c, DEC);

    counter = (counter + 1) % 2;
    
    if(counter == 1) {
      m_rHigh = rx(c);
      DEBUGP(F("hamm high: "));
      DEBUGPLN(m_rHigh, DEC);
    } else if (counter == 0) {
      m_rLow = rx(c);
      DEBUGP(F("hamm low: "));
      DEBUGPLN(m_rLow, DEC);

      if(m_rHigh == m_messageError) {
        DEBUGPLN(F("high has error"));
      }

      if(m_rLow == m_messageError) {
        DEBUGPLN(F("low has error"));
      }

      if(m_rHigh != m_messageError && m_rLow != m_messageError) {
        uint8_t r = ((m_rHigh << 4) & 0xF0) | m_rLow;
        
        if(m_receivingMessage) {
          if(r != m_messageEnd) {
            DEBUGPLN(F("msgRecv"));
            m_message[m_messageIndex] = r;
            m_messageIndex = (m_messageIndex + 1) % SIZE;
          } else {
            DEBUGPLN(F("msgEnd"));
            m_receivingMessage = false;
            m_messageReady = true;
          }
        } else if(r == m_messageBegin) {
          DEBUGPLN(F("msgBegin"));
          m_receivingMessage = true;
        } else {
          DEBUGPLN(F("skipping message"));
        }
      }
    }
  }

  return m_messageReady;
}

void Hamming::read(char* message) {
  strcpy(message, (char*)m_message);
  clearMessage();
}

void Hamming::read(uint8_t& b) {
  char buffer[SIZE];
  read(buffer);

  b = (uint8_t)buffer[0];
}

void Hamming::read(int& i) {
  typedef union {
    int theInt;
    uint8_t theByte[2];
  } byteInt;
  
  byteInt foo;
  foo.theByte[1] = m_message[1];
  foo.theByte[0] = m_message[0];

  i = foo.theInt;
  clearMessage();
}

void Hamming::read(float& f) {
  typedef union {
    float theFloat;
    uint8_t theByte[4];
  } byteFloat;
  
  byteFloat foo;
  foo.theByte[3] = m_message[3];
  foo.theByte[2] = m_message[2];
  foo.theByte[1] = m_message[1];
  foo.theByte[0] = m_message[0];

  f = foo.theFloat;
  clearMessage();
}

void Hamming::read(double& d) {
  typedef union {
    float theDouble;
    uint8_t theByte[4];
  } byteDouble;
  
  byteDouble foo;
  foo.theByte[3] = m_message[3];
  foo.theByte[2] = m_message[2];
  foo.theByte[1] = m_message[1];
  foo.theByte[0] = m_message[0];

  d = foo.theDouble;
  clearMessage();
}
