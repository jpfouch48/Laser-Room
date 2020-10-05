# Laser-Room
This project contains the code and configuration to setup an ESP8266 to monitor the environment of your laser cutter to assure the proper temp settings for your chiller based on room temperature, humidity and dew point. It also contains basic RGB funtionality to control the LED's in your system.

## Required Components:
* Controller - [D1 Mini](https://www.amazon.com/gp/product/B07KW54YSK/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* Room Temp Sensor - [DHT22](https://www.amazon.com/gp/product/B07WP4VZTH/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* Chiller Temp Sensor - [DS18B20](https://www.amazon.com/gp/product/B087JQ6MCP/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* RGB Led Strip - [WS2812B](https://www.amazon.com/gp/product/B01CDTEID0/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

## ESP8266 - Laser Room Configuration
TODO


## Home Assistant Configuration:

### Room Temp Sensor
'''
sensor:
  - platform: mqtt
    state_topic: 'laser/room_temp_sensor'
    name: 'Room Temperature'
    unit_of_measurement: 'F'
    value_template: '{{ value_json.temperature }}'

  - platform: mqtt
    state_topic: 'laser/room_temp_sensor'
    name: 'Room Humidity'
    unit_of_measurement: '%'
    value_template: "{{ value_json.humidity }}"

  - platform: mqtt
    state_topic: 'laser/room_temp_sensor'
    name: 'Room Dew Point'
    unit_of_measurement: 'F'
    value_template: '{{ value_json.dew_point }}"
'''
### Chiller Temp Sensor
'''
sensor:
  - platform: mqtt
    state_topic: 'laser/chiller_temp_sensor'
    name: 'Chiller Temperature'
    unit_of_measurement: 'F'
    value_template: '{{ value_json.temperature }}'
'''
### RGB Lighting


## Wiring
TODO


## Future Enhancements
- [] TODO