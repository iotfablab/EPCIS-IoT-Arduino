# EPCIS-IoT-Arduino

This repository sends sensor information to MQTT Broker in the [InfluxDB Line Protocol Format](https://docs.influxdata.com/influxdb/v1.7/write_protocols/line_protocol_reference/) for Topics that are created based on data from [EPCIS](https://en.wikipedia.org/wiki/EPCIS) Vocabulary.

## Hardware
* __ESP32-WROOM__

## Software
* `arduino-esp32` library
* `PubSubClient` library
* `Adafruit_BME280` library

## Concept

In production environments, products are tracked using __EPC__ (Electronic Product Code) via Barcodes or QR-Codes. Using EPCIS Data Model the following questions are answered for
the product underconsideration:

* __WHAT__
* __WHERE__
* __WHEN__
* __WHY__

IoT sensors cannot be directly connected to products in many cases. Environmental parameters which might affect the product need to be associated with it. For e.g.:

> What was the environment like _when_ the product was somewhere within the production facility?

### Topic Creation

Usage of the following vocabulary from EPCIS can be used to create topics:

| Vocabulary | Example |
|------------|---------|
| __company__| _acme_  |
| __site__   | _factory_ |
| __country code__ | _US_ |
| __city__ | _dallas_ |

Topic: `<company>/<site>/<country_code>/<city>/<sensor_node_mac_address>/<measurement_type>`

`<measurement_type>` is dependent on the type of sensor measurement.E.g.: `env` for Environmental measurements like temperature, humidity, atm. pressure etc.
`<sensor_node_mac_address>` is the unique MAC address of the WLAN/BLE Chip on the node

__Example__

    acme/factory/US/dallas/AA:BB:DE:AD:BE:EF/env

### Payload

In general a very simple Line Protocol format is sent as MQTT payload. The Line Protocol is as follows:

    env,sType=BME280 temp=24.3,humid=59.3 <unix_epoch_timestamp_in_seconds>

`sType`: is the name of the sensor where the data is generated from.
`<unix_epoch_timestamp_in_seconds>`: obtained from SNTP Server.

## Usage
* MQTT packets generally send the payload and topic together to the broker
* A Wildcard topic can be used to subscribe to all sensors within a production area

        <company_name>/<site>/<country_code>/<city>/+/<measurement_name>
    
    E.g.:

        acme/factory/US/dallas/+/env
    
    provides all the sensor values within the Acme Factory situated in Dallas, US

* A simple parsing mechanism / script can subscribe to the topic and extract location information from the topic and add them as `tags` to the Line Protocol _before sending_ it to InfluxDB.

* A simple _Map_ or _dictionary_ is used before sending the data to InfluxDB to associate Sensor Hardware (MAC) Address to a Business Location (`bizLocation`)
    E.g.: Mapping Sensor MAC Address to Business Location
    
        {
            'AA:BB:DE:AD:BE:EF': 'urn:id:loc:sgln:ACME.1'
        }


* E.g.: After parsing the topic and mapping the sensor hardware address to resp. business location, the Line Protocol Payload appears as follows:

        env,sType=BME280,company=acme,site=factory,countryCode=US,city=dallas,sId=AA:BB:DE:AD:BE:EF,bizLocation=urn:id:loc:sgln:ACME.1 temp=24.3,humid=59.3 <unix_epoch_timestamp_in_seconds>

### Example
* `EpcisMqttIoT` example provides the Arduino Sketch to achieve the mentioned concept.
* Within `Settings.h` adapt the configurations necessary:
    * WLAN Configuration
    * MQTT Configuration
    * Topic Metadata Configuration
* Use it with `BME280` to send Temperature, Humidity values

## Querying Logic

You can now query sensor information based on indices from the EPCIS vocabulary as follows:

1. Get environmental information for Acme Factory situated in Dallas:

        SELECT temp, humid FROM env WHERE "company"='acme' AND "site"='factory' AND "city"='dallas'

2. Get environmental information for _business location 1_ in Acme Factory for the last hour:

        SELECT temp, humid FROM env WHERE "company"='acme' AND "site"='factory' AND "bizLocation"='urn:id:loc:sgln:ACME.1' AND time > now() - 1h


## Maintainer
* [Shantanoo Desai](http://github.com/shantanoo-desai)

## License

__Apache 2.0 License__

```
Copyright 2019 BIBA-Bremer Institut für Produktion und Logistik GmbH, IoTFabLab

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
```