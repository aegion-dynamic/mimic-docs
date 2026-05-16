# Memory Management

## Overview

Mimic manages memory by partitioning the STM32's internal SRAM for different system requirements. The goal is to provide enough space for large sensor register shadows while maintaining a stable stack for firmware execution.

## Memory Partitioning

- **Static Allocation:** Core system structures and peripheral configurations are allocated at boot time.
- **Register Shadows:** A dedicated section of SRAM is used to store the virtual register maps for emulated sensors (MPU6050, BMP280, etc.).
- **Command Buffers:** Circular buffers handle incoming and outgoing serial data to prevent blocking during host communication.

## Memory Layout

| Section | Description |
| :--- | :--- |
| **Flash** | Persistent storage for firmware code and default sensor configurations. |
| **SRAM** | Runtime data, including sensor registers and communication buffers. |
| **Stack** | Used for local variables and interrupt context saving. |

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
