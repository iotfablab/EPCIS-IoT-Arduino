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

 * EpcisIot.h: Header file for EpcisIot application
 *             using ESP32-WROOM along side BME280
 * Author: Shantanoo Desai
 * Version: 1.0
*/

/**
 * Class: lineprotocol
 * Purpose: To create InfluxDB's Line Protocol string
 *          consisting of `fields` and `tags` and timestamps
 */

#ifndef EpcisIot_h
#define EpcisIot_h

#include "Arduino.h"

class lineprotocol {
  public:
    lineprotocol(String meas); // Constructor
    String measurement_name;
    
    void addField(String key_name, float value); // Add float Field to Measurement
    void addField(String key_name, int value); // Add int Field to Measurement
    void addTag(String tag_name, String value); // Add Tag to Measurement
    void addTimeStamp_s(uint32_t timestamp_seconds); // Add Epoch Timestamp in Seconds Precision to Measurement
    void addTimeStamp_ms(uint64_t timestamp_milliseconds); // Add Epoch Timestamp in milli-seconds Precision to Measurement
  
    String showLineProtocol(); // show the created Line Protocol String
    void clearLineProtocol(); // Empty the Line Protocol String
    ~lineprotocol(); // Destructor

  private:
    String _data;
    String _tag;
    String _uint64ToString(uint64_t epoch_timestamp);
    
};

#endif
