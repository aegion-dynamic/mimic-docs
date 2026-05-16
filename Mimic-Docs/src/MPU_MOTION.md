# MPU6050 Motion Sensor

## Overview

The MPU6050 is a widely used 6-axis motion tracking device that combines a 3-axis gyroscope and a 3-axis accelerometer. Mimic emulates the I2C interface and register structure of this sensor, allowing you to test flight controllers or navigation algorithms without physical hardware.

## Emulated Features

- **Accelerometer:** X, Y, and Z axis readings with configurable sensitivity.
- **Gyroscope:** Angular velocity readings for all three axes.
- **Temperature:** Internal die temperature register.
- **Interrupts:** Emulation of the Data Ready (DRDY) interrupt signal on a physical GPIO pin.

## Default Configuration

| Parameter | Value |
| :--- | :--- |
| **I2C Address** | 0x68 (default) |
| **Update Rate** | 100 Hz |
| **Resolution** | 16-bit |

## Usage

Start the MPU6050 emulation using the CLI:
```bash
mimic simulate mpu6050
```

Once running, the Mimic hardware will respond to I2C requests from a Master device as if a real MPU6050 were connected to pins **PB6 (SCL)** and **PB7 (SDA)**.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
