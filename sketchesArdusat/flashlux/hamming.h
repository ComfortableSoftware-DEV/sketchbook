/*
  hamming.h - hamming(8,4) ECC library for Arduino.
  Copyright (c) 2016 Jose M Sirvent.  All right reserved.

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

#ifndef HAMMING_H
#define HAMMING_H

#include "Stream.h"
#include <avr/pgmspace.h>

class Hamming
{
public:
  enum { BYTE, INT, FLOAT };
  static const uint8_t SIZE = 32;
  
  Hamming(Stream* m_stream);
  ~Hamming();

  void read(char* message);
  void read(uint8_t& b);
  void read(int& i);
  void read(float& f);
  void read(double& d);
  uint8_t isMessageReady();  
private:
  static const uint8_t m_messageBegin = 0x0F; // 15
  static const uint8_t m_messageEnd   = 0x0D; // 13
  static const uint8_t m_messageError = 0x10; // 16
  
  uint8_t m_rHigh;
  uint8_t m_rLow;
  uint8_t m_receivingMessage;
  uint8_t m_messageReady;
  uint8_t m_message[SIZE];
  uint8_t m_messageIndex;
  Stream* m_stream;

  uint8_t rx(uint8_t data);
  void clearMessage();
};

#endif
