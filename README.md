# Laser-Room
This project contains the code and configuration to setup an ESP8266 to monitor the environment of your laser cutter to assure the proper temp settings for your chiller based on room temperature, humidity and dew point. It also contains basic RGB funtionality to control the LED's in your system.

> :warning: **This component is still under development** Use at your own risk!

## Required Hardware Components:
* Controller          - [D1 Mini](https://www.amazon.com/gp/product/B07KW54YSK/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* Room Temp Sensor    - [DHT22](https://www.amazon.com/gp/product/B07WP4VZTH/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* Chiller Temp Sensor - [DS18B20](https://www.amazon.com/gp/product/B087JQ6MCP/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* RGB Led Strip       - [WS2812B](https://www.amazon.com/gp/product/B01CDTEID0/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

## Required Software Packages:
| Name         | Git Repo                                       | Version      |
|--------------|------------------------------------------------|-------------:|
| OneWire      | PaulStoffregen/OneWire                         | 2.3.5        |
| Dallas Temp  | milesburton/Arduino-Temperature-Control-Library| 3.9.0        |
| DHTesp       | beegee-tokyo/arduino-DHTesp                    | Src Include  |
| Fast LED     | FastLED/FastLED                                | 3.3.0        |
| PubSubClient | knolleary/pubsubclient                         | 2.8.0        |
| ArduinoJson  | bblanchon/ArduinoJson                          | 6.16.1       |
| ArduinoOTA   | jandrassy/ArduinoOTA                           | 1.0.5        |

## ESP8266 - Laser Room Configuration
TODO

## Home Assistant Configuration:

### Room Temp Sensor
```
sensor:
  - platform: mqtt
    state_topic: "laser_room/laser_room_sensor"
    name: "Laser Room Temperature"
    unit_of_measurement: "F"
    value_template: "{{ value_json.temperature }}"

  - platform: mqtt
    state_topic: "laser_room/laser_room_sensor"
    name: "Laser Room Humidity"
    unit_of_measurement: "%"
    value_template: "{{ value_json.humidity }}"

  - platform: mqtt
    state_topic: "laser_room/laser_room_sensor"
    name: "Laser Room Dew Point"
    unit_of_measurement: "F"
    value_template: "{{ value_json.dewpoint }}"
```

### Chiller Temp Sensor
```
sensor:
  - platform: mqtt
    state_topic: "laser_room/laser_room_chiller_sensor"
    name: "Laser Room Chiller Temperature"
    unit_of_measurement: "F"
    value_template: "{{ value_json.temperature }}"
```

### RGB Lighting
```
light:  
  - platform: mqtt  
    schema: json  
    name: "Laser Room LED"  
    state_topic: "laser_room/laser_room_led"  
    command_topic: "laser_room/laser_room_led/set"  
    brightness: true  
    rgb: true  
    white_value: false
    optimistic: false  
    effect: true
    effect_list:
      - solid
      - cylon
      - twinkle
    qos: 0  
```

### RGB Status
```
sensor
  - platform: mqtt
    state_topic: "laser_room/laser_room_status_sensor"
    name: "Laser Room Version"
    value_template: "{{ value_json.version }}"

  - platform: mqtt
    state_topic: "laser_room/laser_room_status_sensor"
    name: "Laser Room IP"
    value_template: "{{ value_json.ip }}"
```

### Lovelace Configuration

#### Status Entry
```
- type: entities
  title: Laser Room Info
  entities:          
    - sensor.laser_room_version
    - sensor.laser_room_ip
```

#### Light Entry
```
- type: custom:light-entity-card
  entity: light.laser_room_led
```

#### Temperature Entry
```
- type: custom:mini-graph-card
  entities:
  - entity: sensor.laser_room_temperature
    name: Laser Room Temp
    color: green              
    show_points: false
  - entity: sensor.laser_room_chiller_temperature
    name: Chiller Temp
    color: red              
    show_points: false
  - entity: sensor.laser_room_dew_point
    name: Dew Point
    color: blue              
    show_points: false
  - entity: sensor.laser_room_humidity
    name: Laser Room Humidity
    color: orange              
    show_points: false
  line_color: blue
  font_size: 100  
  hours_to_show: 1
  points_per_hour: 60
  line_width: 5
```

## Wiring
TODO

## OTA Processing (WIP)
### Arduino IDE
* Install the latest version of python (2.7.8)
* Enable mDNS https://superuser.com/questions/1330027/how-to-enable-mdns-on-windows-10-build-17134
* Install windows 10 Bonjour service (2.2.0)
* Install Bonjour browser

### VS Code

## Future Enhancements
- [X] Complete HA documentation
- [X] Create status component to send general info back to HA
- [X] Add effects to LED component
- [X] Upgrade to ArduinoJson 6
- [X] Add over the air updates
- [ ] Update to have arduino send effect list to be more dynamic
- [ ] Add zones to LEDs to allow different zones to be controlled independently
- [ ] Add ability to control without the need for Home Assistant
- [ ] Add support for PIR to turn on leds when motion is detected
