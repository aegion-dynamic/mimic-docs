# Sensors Overview

The MimicSensors package provides Python-based sensor emulation modules. Each module simulates the behavior and register map of a real-world sensor, allowing the Mimic hardware bridge to present itself as a genuine device on I2C, SPI, or UART buses.

## Supported Sensors

| Sensor | Protocol | Description |
|--------|----------|-------------|
| MPU6050 | I2C | 6-axis accelerometer and gyroscope |
| BMP280 | SPI / I2C | Barometric pressure and temperature sensor |
| GPS (NMEA) | UART | GPS receiver outputting standard NMEA sentences |

## Architecture

All sensor modules inherit from the `AbstractSensorBase` class, which provides:

- **Register Map Management** — A dictionary-based register shadow with read/write hooks.
- **Data Generation** — Configurable data patterns (static, sinusoidal, random, file-based).
- **Update Loop** — A timer-driven update mechanism that refreshes sensor values at configurable rates.

## Quick Start

```bash
# Emulate an MPU6050 with default settings
mimic simulate mpu6050

# Emulate a BMP280 at a custom update rate
mimic simulate bmp280 --rate 100

# Stream GPS NMEA sentences
mimic simulate gps --trajectory default
```

## Adding Custom Sensors

To create a new sensor module, extend `AbstractSensorBase` and implement the required methods:

1. Define the register map
2. Implement the data generation logic
3. Register the sensor with the CLI plugin system

See the [Abstract Sensor Base](./SENSOR_BASE.md) chapter for the full API reference.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
