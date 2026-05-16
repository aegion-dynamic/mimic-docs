# I2C Timing & Speed

## Overview

Accurate timing is critical for I2C communication. Mimic provides fine-grained control over the bus frequency and signal timing to ensure compatibility with a wide range of master devices.

## Supported Speeds

- **Standard Mode:** 100 kHz
- **Fast Mode:** 400 kHz
- **Fast Mode Plus:** Up to 1 MHz (hardware dependent)

## Timing Accuracy

The firmware utilizes the STM32's internal peripheral clock to generate I2C signals. By configuring the rise and fall time parameters, the system can emulate the characteristics of different sensor hardware.

## Clock Stretching

If the host bridge is slow to provide data for a transaction, the firmware can use I2C clock stretching to pause the master until the data is ready, preventing bus errors.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
