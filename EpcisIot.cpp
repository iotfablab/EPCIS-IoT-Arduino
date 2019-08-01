/**
 *  Copyright 2019 BIBA-Bremer Institut für Produktion und Logistik GmbH, IoTFabLab

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

 * EpcisIot.cpp: Implementation file for EpcisIot application
 *             using ESP32-WROOM along side BME280
 * Author: Shantanoo Desai
 * Version: 1.0
 */

#include "Arduino.h"
#include "EpcisIot.h"


/**
 * lineprotocol class methods
 */

// Constructor
lineprotocol::lineprotocol(String meas)
{
  measurement_name = meas;
}

void lineprotocol::addField(String key_name, float value) 
{
  _data = (_data == "") ? " " : (_data += ",");
  _data += key_name + "=" + String(value);
}

void lineprotocol::addField(String key_name, int value)
{
  _data = (_data == "") ? " " : (_data += ",");
  _data += key_name + "=" + String(value) + "i";
}

void lineprotocol::addTag(String tag_name, String value)
{
  _tag += "," + tag_name + "=" + value; 
}

void lineprotocol::addTimeStamp_s(uint32_t timestamp_seconds)
{
  if (timestamp_seconds > 1) { // higher than 1/1/1970 Epoch Beginning
    _data += " " + String(timestamp_seconds);
  }
}

void lineprotocol::addTimeStamp_ms(uint64_t timestamp_milliseconds) 
{
  if (timestamp_milliseconds > 1) { // higher than 1/1/1970 Epoch Beginning
    // need to convert uint64_t timestamp to string
    String _result = _uint64ToString(timestamp_milliseconds);
    _data += " " + _result;
  }
}

// display created line protocol
String lineprotocol::showLineProtocol() {
  return measurement_name + _tag + _data;
}

String lineprotocol::_uint64ToString(uint64_t value) 
{
  String result = "";
  uint8_t base = 10;

  do {
    char c = value % base;
    value /= base;

    if (c < 10) {
      c += '0';
    } else {
      c += 'A' - 10;
    }
    result += c;
    
  } while(value);
  
  return result;
}

lineprotocol::~lineprotocol() 
{
}
