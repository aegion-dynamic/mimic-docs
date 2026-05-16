# I2C Register Emulation

## Overview

Mimic's core strength is its ability to emulate the register-based behavior of I2C sensors. It maintains a virtual register map that responds to standard I2C read and write transactions.

## Register Shadowing

The firmware keeps an array in SRAM that mirrors the registers of the sensor being emulated.
- **Master Write:** The master sends an address and data; the firmware updates the corresponding shadow register.
- **Master Read:** The master requests data from an address; the firmware retrieves the value from the shadow register and transmits it.

## Auto-Increment

The emulation supports auto-incrementing addresses, allowing the master to read multiple sequential registers (e.g., Accelerometer X, Y, Z values) in a single transaction.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
