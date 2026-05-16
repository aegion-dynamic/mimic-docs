# GPIO Mapping

## Overview

GPIO Mapping defines how the logical pins in the Mimic firmware are connected to the physical pins on the STM32F411 Black Pill. This mapping ensures that commands from the host reach the correct physical hardware.

## Pin Configuration

The firmware uses a standard mapping for peripheral functions:
- **I2C1:** SCL (PB6), SDA (PB7)
- **SPI1:** SCK (PA5), MISO (PA6), MOSI (PA7), CS (PA4)
- **UART1 (GPS):** TX (PA9), RX (PA10)
- **UART2 (Bridge):** TX (PA2), RX (PA3)
- **User LED:** PC13

## Custom Mapping

While the default mapping is recommended, users can modify the `gpio_map.h` file in the firmware source to reassign pins if their specific hardware setup requires it.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
