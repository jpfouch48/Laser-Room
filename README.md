# Laser-Room
This project contains the code and configuration to setup an ESP8266 to monitor the environment of your laser cutter to assure the proper temp settings for your chiller based on room temperature, humidity and dew point. It also contains basic RGB funtionality to control the LED's in your system.

> :warning: **This component is still under development** Use at your own risk!

## Required Components:
* Controller - [D1 Mini](https://www.amazon.com/gp/product/B07KW54YSK/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* Room Temp Sensor - [DHT22](https://www.amazon.com/gp/product/B07WP4VZTH/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* Chiller Temp Sensor - [DS18B20](https://www.amazon.com/gp/product/B087JQ6MCP/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
* RGB Led Strip - [WS2812B](https://www.amazon.com/gp/product/B01CDTEID0/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)

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
    value_template: "{{ value_json.dew_point }}"
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
    qos: 0  
```

### Lovelace Configuration
#### Light entry
```
- type: custom:light-entity-card
  entity: light.laser_room_led
```

#### Temperature Entry
```
- type: custom:mini-graph-card
  entities:
  - entity: sensor.laser_room_temperature
    name: Temperature
  - entity: sensor.laser_room_humidity
    name: Humidity
  - entity: sensor.laser_room_dew_point
    name: Dew Point
  - entity: sensor.laser_room_chiller_temperature
    name: Chiller Temperature
  - color: gray
    entity: input_number.nighttime
    name: Night
    show_line: false
    show_points: false
    show_legend: false
    y_axis: secondary
  show:
    labels: true
    labels_secondary: true
```


## Wiring
TODO

## OTA Processing
TODO

## Future Enhancements
- [ ] Add over the air updates
- [ ] Complete HA documentation
- [ ] Add support for PIR to turn on leds when motion is detected