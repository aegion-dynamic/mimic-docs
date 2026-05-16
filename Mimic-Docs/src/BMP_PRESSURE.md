# BMP280 Environment Sensor

## Overview

The BMP280 is an absolute barometric pressure sensor designed for mobile applications. Mimic provides a highly accurate emulation of its SPI and I2C interfaces, supporting both pressure and temperature readings.

## Emulated Features

- **Pressure Sensing:** Supports various oversampling modes to simulate different levels of noise and resolution.
- **Temperature Sensing:** Integrated temperature register for thermal compensation logic testing.
- **Protocol Support:** Can be configured to respond over either I2C or SPI depending on your test requirements.

## Pin Mapping (SPI Mode)

- **SCK:** PA5
- **MISO:** PA6
- **MOSI:** PA7
- **CS:** PA4

## Usage

Start the BMP280 emulation in SPI mode:
```bash
mimic simulate bmp280 --protocol spi
```

The emulation includes the factory calibration registers, ensuring that your driver's compensation formulas can be fully validated.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
