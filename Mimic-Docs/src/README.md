# Welcome to MIMIC

Mimic is a framework designed to bridge the gap between software development and physical hardware by emulating sensor behavior. It allows a host computer to interact with a microcontroller that "mimics" the protocols and data streams of actual hardware peripherals.

## Core Concept
The primary purpose of Mimic is to provide a functional replacement for sensors during development or when hardware is unavailable. The system uses a dedicated microcontroller to act as a sensor proxy, supporting multiple communication protocols including **I2C, SPI, UART, RS485, and RS232**.

### Hardware Platform
Mimic Firmware is developed specifically for the **STM32F411CEU6 (Black Pill)** development board. It is designed to run exclusively on this platform to ensure consistent peripheral timing and protocol accuracy.

## System Architecture
The framework is divided into three distinct layers:

### 1. Mimic Firmware
Written in C, the firmware resides on the STM32 MCU. It is responsible for parsing commands received from the host computer and executing them on the hardware level. It manages the low-level peripheral configuration required to emulate different sensor interfaces.

### 2. Mimic Bridge & CLI
The **Mimic Bridge** is a simulation ecosystem implemented in Python. It provides the interface for the computer to communicate with and program the Mimic MCU in real-time. 

Accompanying the bridge is the **Mimic CLI**, which allows for direct hardware interaction. You can use the CLI to:
- Manually control the MCU pinout.
- Query the real-time status of GPIO pins.
- Execute protocol commands directly from the terminal.

### 3. Mimic Sensors
**Mimic Sensors** contains the actual emulation logic for specific hardware. This is where the behavior of a sensor (such as an MPU6050 or BMP280) is defined. This layer is designed for community contribution, allowing users to write and add new sensor profiles to the ecosystem.

---
*Select a topic from the sidebar to explore the documentation for each specific module.*
