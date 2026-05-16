# Command Line Interface (CLI)

The Mimic framework provides a standardized CLI and Python library structure (`mimic-fw`) to interface seamlessly with Mimic hardware. 

The CLI is powered by the Python `click` module, establishing a centralized entry point. Running `mimic` without arguments attempts an automatic discovery of the connected board and enters an interactive Gruvbox-themed shell.

## Basic Usage

### Auto-Discovery Shell
Launch the shell and let the library automatically detect the connected BlackPill:
```bash
mimic
```

### Specifying Hardware Ports
If you have multiple devices connected, specify the USB port explicitly:
```bash
# Linux/macOS
mimic --port /dev/ttyUSB0

# Windows
mimic --port COM3
```

## Common Board Commands

Once inside the interactive terminal, you can manually type out protocol instructions to the STM32 board. Some common commands include:

*   **`STATUS`** - Retrieves the current MCU system state.
*   **`VERSION`** - Retrieves firmware build date and internal version.
*   **`PIN_HIGH [PIN]`** - Configures the specified GPIO pin (e.g. `PC13`) to logical HIGH.
*   **`PIN_LOW [PIN]`** - Configures the specified GPIO pin to logical LOW.
*   **`RESET`** - Soft-reboots the hardware.

## Quick Simulations
The CLI supports rapid spawning of Mock Sensor data. This is useful if you are developing another application on an external Master device (e.g., Raspberry Pi) and need the BlackPill to pretend to be a sensor.

```bash
# Starts mimicking an MPU6050 Accelerometer
mimic simulate mpu6050

# Starts mimicking a BMP280 over I2C instead of SPI
mimic simulate bmp280 --protocol i2c

# Emulates a running GPS over UART NMEA sentences
mimic simulate gps
```
