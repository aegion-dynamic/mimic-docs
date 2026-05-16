# System Limitations

## Overview

While Mimic is a powerful tool for hardware emulation, it is important to understand its physical and logical limits to ensure accurate testing.

## Hardware Constraints

- **Logic Level:** Mimic operates at 3.3V. Interfacing with 5V systems without level shifters may damage the STM32.
- **Max Frequency:** I2C is limited to 1 MHz and SPI to approximately 20 MHz due to CPU and DMA overhead.
- **Pin Current:** GPIO pins have limited current sourcing/sinking capability (typically 20mA). Do not drive motors or high-power LEDs directly.

## Emulation Limits

- **Timing Jitter:** While minimal, some jitter is inherent in a software-driven emulation compared to physical ASIC hardware.
- **Register Set:** Not all registers of every sensor are emulated. Only the most commonly used registers for data retrieval and configuration are typically supported.
- **Analog Features:** Mimic is primarily a digital protocol emulator. It does not natively emulate complex analog sensor behaviors (like ADC noise characteristics) unless specifically modeled in the Python layer.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
