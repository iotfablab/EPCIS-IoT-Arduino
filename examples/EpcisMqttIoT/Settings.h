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
   
   Author: Shantanoo Desai
   Version: 1.0
*/

#ifndef Settings_h
#define Settings_h

/*WLAN Configuration Settings*/
const char* ssid = "Your_SSID_WLAN";
const char* password = "Password_WLAN";

/*MQTT Broker Configuration Settings*/
const char* mqtt_broker_address = "Your_MQTT_Broker_Address";
const uint16_t mqtt_port = 1883;
const char* client_id = "ESP32_1";

/*MQTT Topic Parameters*/
// NOTE: All Names should be lower-case without special alphabets (NO: ö/Ö/ä/Ä/ü/Ü/)
String company_name = "acme";
String site_name = "factory";
String city_name = "dallas";

//NOTE: All Country Codes should be in Capital Letters e.g. 'DE', 'US', 'SE' etc.
String country_code = "US";

// Sensor Information
String sName= "BME280";
String sType = "env"; // Sensor Measurement Type (e.g. "env" for environmental)

/*NTP Server Configuration Settings*/
const char* ntp_server = "pool.ntp.org"; // NOTE: Always prefer UTC Time

/*line protocol stags*/
#endif
