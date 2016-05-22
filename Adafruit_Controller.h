/**************************************************************************/
/*!
    @file     Adafruit_Controller.h
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

#ifndef Adafruit_Controller_h
#define Adafruit_Controller_h

#include <Arduino.h>
#include "packet_format.h"

typedef void (*threeAxisHandlerFuncPtr) (float, float, float);
typedef void (*quatHandlerFuncPtr) (float, float, float, float);
typedef void (*buttonHandlerFuncPtr) (byte, bool);
typedef void (*colorHandlerFuncPtr) (byte, byte, byte);

class Adafruit_Controller {
  private:
  Stream &comm;
  packet_t packet_buffer;
  int packet_idx = 2;

  threeAxisHandlerFuncPtr accelHandler = NULL;
  threeAxisHandlerFuncPtr gyroHandler = NULL;
  threeAxisHandlerFuncPtr magHandler = NULL;
  threeAxisHandlerFuncPtr locationHandler = NULL;
  quatHandlerFuncPtr quatHandler = NULL;
  colorHandlerFuncPtr colorHandler = NULL;
  buttonHandlerFuncPtr buttonHandler = NULL;
  
  bool isValidPacket();
  
  public:

  Adafruit_Controller(Stream &ble);

  // read input and see if we have a complete packet
  bool process();
  void printPacket(Stream &stream);

  // register callback functions
  void handleAccelerometer(threeAxisHandlerFuncPtr handler);
  void handleButton(buttonHandlerFuncPtr handler);
  void handleColor(colorHandlerFuncPtr handler);
  void handleGyroscope(threeAxisHandlerFuncPtr handler);
  void handleMagnetometer(threeAxisHandlerFuncPtr handler);
  void handleLocation(threeAxisHandlerFuncPtr handler);
  void handleQuaternion(quatHandlerFuncPtr handler);

};

#endif