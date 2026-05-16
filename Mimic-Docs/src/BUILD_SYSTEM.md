# Build System

## Overview

The Mimic firmware uses a standard Makefile-based build system designed for the GNU Arm Embedded Toolchain. This ensures that the code can be compiled and deployed from any standard Linux, macOS, or Windows terminal.

## Requirements

To build the firmware, you need:
- **arm-none-eabi-gcc:** The cross-compiler for ARM Cortex-M microcontrollers.
- **make:** The build automation tool.
- **st-flash (or OpenOCD):** For uploading the binary to the STM32 board.

## Compilation Commands

- **`make`** — Compiles the source files and generates the binary image.
- **`make clean`** — Removes all compiled objects and binaries.
- **`make flash`** — Compiles and uploads the code to the connected STM32 via an ST-Link.

## Output Files

The build process produces several files in the `build/` directory:
- `mimic.bin` — The raw binary image for flashing.
- `mimic.elf` — The executable file containing debug symbols.
- `mimic.hex` — The Intel Hex format version of the binary.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
