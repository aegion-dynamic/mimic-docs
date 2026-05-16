# MPU6050 Register Map

## Overview

The MPU6050 emulation includes a complete register map that mimics the physical device. This allows your software to configure the "virtual" sensor just as it would a real one.

## Key Registers

- **0x75 (WHO_AM_I):** Returns `0x68` to confirm device identity.
- **0x3B - 0x40 (ACCEL_XOUT):** 16-bit accelerometer readings.
- **0x43 - 0x48 (GYRO_XOUT):** 16-bit gyroscope readings.
- **0x6B (PWR_MGMT_1):** Controls the device's power state and clock source.

## Interaction

When your master device writes to a register, Mimic updates the internal shadow. For example, writing to the `GYRO_CONFIG` register will change how the emulation calculates angular velocity data.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
