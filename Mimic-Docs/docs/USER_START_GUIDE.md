# Mimic User Start Guide

Follow these steps to develop and test with Mimic.

## 1. Programming the User Device (ESP8266)
Use the `arduino-cli` binary in the project root.

**Compile & Upload:**
```bash
./bin/arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 Esp-Test/MPU6050.ino && \
./bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn esp8266:esp8266:nodemcuv2 Esp-Test/MPU6050.ino
```

**Monitor Output:**
```bash
./bin/arduino-cli monitor -p /dev/ttyUSB0 --config baudrate=115200
```
*Note: Close the monitor (Ctrl+C) before uploading new code.*

---

## 2. Starting the Sensor Simulation
On your laptop, the `mimic` command acts as the "Brain" for the box.

**Run MPU6050:**
```bash
sudo mimic mpu6050
```

---

## 3. Updating the Mimic Box (STM32)
If you modify the C code in `firmware/Core/Src/mimic.c`, you must re-flash the hardware.

**Flash via ST-Link:**
```bash
cd firmware
make clean && make flash
```

---

## 4. Common Troubleshooting
*   **"No Mimic board found"**: Unplug and re-plug the STM32 USB cable. It may have crashed the serial bridge or changed port names.
*   **"Device or resource busy"**: A Serial Monitor is already open. Close it.
*   **Constant -57.80 readings**: Signal wires are loose or disconnected. Mimic is serving "Ghost Data." 
