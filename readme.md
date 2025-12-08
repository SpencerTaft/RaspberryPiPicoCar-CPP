This is a simple README for the last raspberry pi pico w version.
Future releases will be developed for esp32 targets.

Features:
- ability to add light runtimes, which can update pin voltages
- http server that can receive HTTP requests containing JSON updates for light runtimes
- PWM lighting with ramp function for configurable flashing lights

Future goals:
- exception handling
- improve code quality
- use the dual cores to run realtime light updates (eg. flashing)

Building:
Add a folder named "external" containing the folders "json" and "pico-sdk".
The json folder should contain nlohmann::json, https://github.com/nlohmann/json.git 
The pico-sdk should contain the pico sdk, https://github.com/raspberrypi/pico-sdk.git
Then the following build commands can be used:
mkdir build && cd build
rm -rf ./*
#cmake -DPICO_BOARD=pico_w ..
PICOHTTPS_WIFI_PASSWORD=yourpassword cmake -D"PICO_BOARD=pico_w" ..
make

CURL Commands to set left and right headlights:
//current turn on light
curl -X POST http://10.0.0.104:4242 -d '{"config1": {"ID": "left headlight", "pin": 14, "isOn": true}}' -H "Content-Type: application/json"
//current turn off light
curl -X POST http://10.0.0.104:4242 -d '{"config1": {"ID": "left headlight", "pin": 14, "isOn": false}}' -H "Content-Type: application/json"

//current turn on light
curl -X POST http://10.0.0.104:4242 -d '{"config1": {"ID": "right headlight", "pin": 15, "isOn": true}}' -H "Content-Type: application/json"
//current turn off light
curl -X POST http://10.0.0.104:4242 -d '{"config1": {"ID": "left headlight", "pin": 15, "isOn": false}}' -H "Content-Type: application/json"

