/**************************************************************************/
/*!
    @file     Adafruit_Controller.cpp
    @author   glowingmonkey

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2016, Nick Lopez
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/


#include "Adafruit_Controller.h"

Adafruit_Controller::Adafruit_Controller(Stream &ser) : comm(ser) {}

bool Adafruit_Controller::process()
{
  if (comm.available())
  {
    char in = comm.read();
    if (in == '!') { // start of packet
      packet_idx = 2;
      memset(packet_buffer.buffer, 0, 21);
    }
    packet_buffer.buffer[packet_idx++] = in;
    if (isValidPacket())
    {
      switch (packet_buffer.pkt.action) {
        case 'A':
          if (accelHandler)
            accelHandler(packet_buffer.pkt.type.accel.x, packet_buffer.pkt.type.accel.y, packet_buffer.pkt.type.accel.z);
          break;
        case 'G':
          if (gyroHandler)
            gyroHandler(packet_buffer.pkt.type.gyro.x, packet_buffer.pkt.type.gyro.y, packet_buffer.pkt.type.gyro.z);
          break;
        case 'M':
          if (magHandler)
            magHandler(packet_buffer.pkt.type.mag.x, packet_buffer.pkt.type.mag.y, packet_buffer.pkt.type.mag.z);
          break;
        case 'Q':
          if (quatHandler)
            quatHandler(packet_buffer.pkt.type.quat.x, packet_buffer.pkt.type.quat.y, packet_buffer.pkt.type.quat.z, packet_buffer.pkt.type.quat.w);
          break;
        case 'B':
          if (buttonHandler) {
            byte btn = packet_buffer.pkt.type.button.button - '0';
            bool state =  packet_buffer.pkt.type.button.state == '1' ? true : false;
            buttonHandler(btn, state);
          }
          break;
        case 'C':
          if (colorHandler)
            colorHandler(packet_buffer.pkt.type.color.red, packet_buffer.pkt.type.color.green, packet_buffer.pkt.type.color.blue);
          break;
        case 'L':
          if (locationHandler)
            locationHandler(packet_buffer.pkt.type.location.lat, packet_buffer.pkt.type.location.lon, packet_buffer.pkt.type.location.alt);
          break;
        default: // unknown type
          return false;
          break;
      }
      return true;
    }
  }
  return false;
}

bool Adafruit_Controller::isValidPacket()
{
  byte full_length = 0;
  switch (packet_buffer.pkt.action) {
    case 'A':
      full_length = 15;
      break;
    case 'G':
      full_length = 15;
      break;
    case 'M':
      full_length = 15;
      break;
    case 'Q':
      full_length = 19;
      break;
    case 'B':
      full_length = 5;
      break;
    case 'C':
      full_length = 6;
      break;
    case 'L':
      full_length = 15;
      break;
    default: // unknown type
      return false;
      break;
  }
  if ((packet_idx - 2) == full_length) {
    // check checksum!
    uint8_t xsum = 0;
    uint8_t checksum = packet_buffer.buffer[packet_idx - 1];

    for (uint8_t i = 2; i < packet_idx - 1; i++) {
      xsum += packet_buffer.buffer[i];
    }
    xsum = ~xsum;

    // Throw an error message if the checksum's don't match
    if (xsum != checksum)
    {
      Serial.println("Checksum mismatch in packet");
      return false;
    }
    return true;
  }

  return false;
}

void Adafruit_Controller::printPacket(Stream &stream) {
  stream.print(packet_buffer.pkt.action);
  switch (packet_buffer.pkt.action) {
    case 'A':
      stream.print("x:");
      stream.print(packet_buffer.pkt.type.accel.x);
      stream.print(" y:");
      stream.print(packet_buffer.pkt.type.accel.y);
      stream.print(" z:");
      stream.print(packet_buffer.pkt.type.accel.z);
      stream.println();
      break;
    case 'G':
      stream.print("x:");
      stream.print(packet_buffer.pkt.type.gyro.x);
      stream.print(" y:");
      stream.print(packet_buffer.pkt.type.gyro.y);
      stream.print(" z:");
      stream.print(packet_buffer.pkt.type.gyro.z);
      stream.println();
      break;
    case 'M':
      stream.print("x:");
      stream.print(packet_buffer.pkt.type.mag.x);
      stream.print(" y:");
      stream.print(packet_buffer.pkt.type.mag.y);
      stream.print(" z:");
      stream.print(packet_buffer.pkt.type.mag.z);
      stream.println();
      break;
    case 'Q':
      stream.print("x:");
      stream.print(packet_buffer.pkt.type.quat.x);
      stream.print(" y:");
      stream.print(packet_buffer.pkt.type.quat.y);
      stream.print(" z:");
      stream.print(packet_buffer.pkt.type.quat.z);
      stream.print(" w:");
      stream.print(packet_buffer.pkt.type.quat.w);
      stream.println();
      break;
    case 'B':
      stream.print("utton: ");
      stream.print(packet_buffer.pkt.type.button.button - '0');
      stream.print(packet_buffer.pkt.type.button.state == '1' ? " Pressed" : " Released");
      break;
    case 'C':
      stream.print(packet_buffer.pkt.type.color.red, HEX);
      stream.print(packet_buffer.pkt.type.color.green, HEX);
      stream.print(packet_buffer.pkt.type.color.blue, HEX);
      break;
    case 'L':
      stream.print("lat:");
      stream.print(packet_buffer.pkt.type.location.lat);
      stream.print(" lon:");
      stream.print(packet_buffer.pkt.type.location.lon);
      stream.print(" alt:");
      stream.print(packet_buffer.pkt.type.location.alt);
      stream.println();
      break;
    default: // unknown type
      stream.print("Unknown packet type: ");
      stream.print(packet_buffer.pkt.action);
      break;
  }
  stream.println();
}

void Adafruit_Controller::handleAccelerometer(threeAxisHandlerFuncPtr handler)
{
  accelHandler = handler;
}
void Adafruit_Controller::handleGyroscope(threeAxisHandlerFuncPtr handler)
{
  gyroHandler = handler;
}
void Adafruit_Controller::handleMagnetometer(threeAxisHandlerFuncPtr handler)
{
  magHandler = handler;
}
void Adafruit_Controller::handleLocation(threeAxisHandlerFuncPtr handler)
{
  locationHandler = handler;
}
void Adafruit_Controller::handleQuaternion(quatHandlerFuncPtr handler)
{
  quatHandler = handler;
}
void Adafruit_Controller::handleColor(colorHandlerFuncPtr handler)
{
  colorHandler = handler;
}
void Adafruit_Controller::handleButton(buttonHandlerFuncPtr handler)
{
  buttonHandler = handler;
}

