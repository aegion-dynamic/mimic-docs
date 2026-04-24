# 🧬 Protocol Specifications

Detailed technical specifications for communication protocols supported by Mimic v1.

---

## 🟦 I2C (Inter-Integrated Circuit)

### Implementation
Mimic uses the STM32 Hardware I2C peripheral.
*   **Standard Mode**: 100 kHz (Recommended)
*   **Fast Mode**: Up to 400 kHz (Supported, but stability depends on cable length and pull-ups).
*   **Slave Mode**: Supports clock stretching. If the Python host is slow to respond, the STM32 will hold the SCL line LOW to pause the master.

### Wiring
*   **I2C1**: SCL (PB8), SDA (PB9)
*   **Voltage**: 3.3V logic.
*   **Pull-ups**: The STM32 enables internal pull-ups, but for reliable communication (especially at 400kHz), **external 4.7kΩ resistors** to 3.3V are strongly recommended.

---

## 🟧 SPI (Serial Peripheral Interface)

### Implementation
*   **Clock Polarity (CPOL)**: 0 or 1
*   **Clock Phase (CPHA)**: 0 or 1
*   **Data Size**: 8-bit or 16-bit
*   **Bit Order**: MSB First (Standard) or LSB First.

### Wiring
*   **SPI1**: SCK (PA5), MISO (PA6), MOSI (PA7).
*   **CS**: Any GPIO can be used as Chip Select via the `SPI_CS` command.

---

## 🟩 UART & RS485

### Hardware DE Management
The RS485 implementation is hardware-assisted. 
1.  When `UART_SEND` is called, the DE pin is pulled HIGH.
2.  The UART data is streamed.
3.  The firmware waits for the `TC` (Transmission Complete) flag in the hardware register to ensure the last bit has physically left the pin.
4.  The DE pin is pulled LOW.

This ensures zero-latency switching between transmit and receive modes, essential for half-duplex RS485 communication.
