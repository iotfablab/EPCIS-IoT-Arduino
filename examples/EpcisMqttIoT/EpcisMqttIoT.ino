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


// For I2C
#include <Wire.h>
// For MQTT
#include<PubSubClient.h>
// For WLAN
#include <WiFi.h>
// For BME280 Sensor
#include <Adafruit_BME280.h>
// For Line Protocol Strings
#include <EpcisIot.h>

// For Application Settings
#include "Settings.h"

#define DELAY_SAMPLING_TIME_SECS   (2000)

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_BME280 bme;


float humid, temp = 0.0;
String topic = ""; // Topic to be Created for Publish
String sID = ""; // Sensor ID (MAC Address of ESP32)



/* Get Timestamp from NTP Server*/
unsigned long getLocalTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("NTP:getLocalTime: Failed to Obtain Time");
    return(0);
  }
  time(&now);
  return now;
}

/*Reconnect to MQTT Broker*/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("MQTT:reconnect: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_id)) {
      Serial.println("MQTT:reconnect: Connected");
    } else {
      Serial.print("MQTT:reconnect: Failed, rc=");
      Serial.print(client.state());
      Serial.println("MQTT:reconnect: Trying Again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin(13, 16); // I2C Pins for ESP32-WROOM
  bme.begin(0x77); // for Sensor BoosterPack from TI BME address on I2C is 77
  Serial.println("main:setup: BME280 setup complete");
  Serial.println("main:setup: Connecting to WLAN");
   WiFi.begin(ssid, password);
   configTime(0, 0, ntp_server);
   while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
   }
   Serial.println("main:setup: WiFi Connected");
   Serial.println("main:setup: IP Address: ");
   Serial.println(WiFi.localIP());
   Serial.println("main:setup: MAC Address: ");
   Serial.println(WiFi.macAddress());
   // Assign Sensor ID
   sID = WiFi.macAddress();
   delay(1000);
   
   // Setup MQTT Configuraton
   Serial.println("main:setup: Setting up MQTT Configuration");
   client.setServer(mqtt_broker_address, mqtt_port);
   // creating topic for Publish
   topic += company_name + "/" + site_name + "/" + country_code + "/" + city_name + "/" + sID + "/" + sType;
   Serial.println("main:setup: MQTT Topic: ");
   Serial.println(topic);
}

void loop() {
  if (!client.connected()){
    reconnect();
  }
  client.loop();
  lineprotocol row(sType); // line protocol: measurement name for BME280 temperature,humidity ("env")
  row.addTag("sName", sName);

  temp = bme.readTemperature();
  humid = bme.readHumidity();
  row.addField("temp", temp);
  row.addField("humid", humid);

  row.addTimeStamp_s(getLocalTime());
  Serial.println(row.showLineProtocol());
  client.publish(topic.c_str(), row.showLineProtocol().c_str());
  delay(DELAY_SAMPLING_TIME_SECS);
}
