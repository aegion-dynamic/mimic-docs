# Peripheral Control

## Overview

Mimic provides direct control over the STM32's hardware peripherals through its bridge protocol. This allows the host to configure and interact with low-level hardware without writing new C code.

## Supported Peripherals

- **GPIO:** Read and write digital states on any available MCU pin.
- **I2C:** Emulate slave devices or act as a master to control external hardware.
- **SPI:** Support for high-speed sensor emulation and data streaming.
- **UART:** Stream serial data or act as a bridge between different serial protocols.

## Control Logic

Peripherals are managed by a dedicated driver layer in the firmware. When a command is received from the host (e.g., `PIN_HIGH PC13`), the firmware identifies the correct peripheral register and updates it to reflect the desired state.

## Configuration

Default peripheral mappings are defined in the firmware's header files. These can be overridden at runtime using the CLI or Python API to adapt to different hardware setups.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
