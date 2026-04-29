# ESP Side 

Esp-Test$ ./bin/arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 BMP280/BMP280.ino && ./bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2 BMP280/BMP280.ino 


./bin/arduino-cli monitor -p /dev/ttyUSB0 --config baudrate=115200

# STM  SIDE

sudo mimic <sensor>

sudo mimic