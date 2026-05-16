# Firmware Overview

The MimicFirmware is the core embedded component of the Mimic system. It runs on an STM32F411CEU6 (BlackPill) microcontroller and is responsible for interpreting host commands, driving peripheral emulation, and maintaining real-time communication with the Python bridge.

## Architecture

The firmware is structured as a bare-metal application with the following core modules:

- **Command Parser** — Receives and tokenizes incoming serial packets from the USB CDC interface or the secondary UART2 (TTL) bridge.
- **GPIO Controller** — Manages pin state changes for digital output, input reading, and interrupt-driven monitoring.
- **I2C Slave Engine** — Emulates I2C slave devices by shadowing register maps and responding to master transactions.
- **SPI Slave Engine** — Handles SPI communication for sensors like the BMP280.
- **UART Bridge** — Provides NMEA sentence streaming for GPS emulation.

## Execution Model

The firmware operates on a single-threaded super-loop model with interrupt-driven peripherals:

1. The main loop polls the USB CDC receive buffer for incoming commands.
2. Commands are parsed and dispatched to the appropriate peripheral handler.
3. Hardware timers and DMA channels handle time-critical operations independently of the main loop.

## Memory Layout

The STM32F411 provides 512KB of Flash and 128KB of SRAM. The firmware memory is partitioned as:

- **Flash (0x08000000)** — Application code and constant data.
- **SRAM (0x20000000)** — Stack, heap, and peripheral register shadows.

## Build Targets

The firmware supports multiple build configurations:

- `make` — Default release build with optimizations.
- `make debug` — Debug build with symbols and no optimization.
- `make flash` — Build and flash in one step (requires ST-Link).

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
