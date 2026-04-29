#  Mimic v1: The hardware Bridge & Mocking Engine

**Mimic** is a high-performance hardware bridge designed for developers who need to write firmware, drivers, or software **before** their hardware arrives. By utilizing an STM32 BlackPill as a versatile "chameleon," Mimic allows you to simulate high-speed sensors, control peripherals, and bridge complex protocols like RS485 directly from your PC.

> [!IMPORTANT]
> **Philosophy**: Don't wait for your sensor to arrive. Mimic a sensor, write your code, and simply swap the wires when the real hardware arrives.

---

## Implementation Architecture

Mimic consists of two synchronized layers:
1.  **The Core (Firmware)**: A highly optimized C/HAL engine running on the **STM32F411CEU6**. It manages real-time interrupts for high-speed UART, I2C Master/Slave logic, and SPI DMA transfers.
2.  **The Shell (Python Library)**: A modern, modular Python 3 package that provides auto-discovery, packet serialization, and an extensible sensor simulation framework.

---

##  Hardware Setup: STM32 BlackPill

### Pinout Reference (CEU6 48-pin)

| Peripheral | Pins | Notes |
| :--- | :--- | :--- |
| **Host CLI** | **PA9 (TX) / PA10 (RX)** | Connect to PC via USB-TTL (115200 Baud) |
| **I2C 1** | **PB8 (SCL) / PB9 (SDA)** | Pull-up resistors required for Slave transactions |
| **SPI 1** | **PA5(SCK) / PA6(MISO) / PA7(MOSI)** | Supports Master & Slave modes |
| **UART 6** | **PA11 (TX) / PA12 (RX)** | Optimized for RS485 with Auto-DE management |
| **UART 2** | **PA2 (TX) / PA3 (RX)** | Generic UART / RS232 buffer |

---

##  Installation

Install the entire suite (Library + Simulator CLI) globally:

```bash
git clone https://github.com/Karthik-Sarvan/Mimic.git
cd Mimic
sudo pip install .
```

---

## Usage Modes

Mimic is designed to be versatile, supporting three distinct workflows:

### A. The Simulator (Act as a Sensor)
Directly mimic a complex device. For example, to act as an **MPU6050** at address `0x68`:
```bash
mimic-sim mpu6050
```

### B. The Python Library (Automation)
Integrate Mimic into your own testing scripts or CI/CD pipelines.

```python
from mimic import MimicBridge
from mimic.sensors.mpu6050 import MPU6050Simulator

# Initialize Bridge (Auto-detects the USB port)
bridge = MimicBridge()
if bridge.connect():
    # Example: Manually toggle a pin
    bridge.execute("PIN_HIGH A5")
    
    # Start the MPU6050 Mock
    sim = MPU6050Simulator(bridge)
    sim.start()
```

### C. The Interactive CLI
Connect via any serial terminal (Baud: 115200) to type commands directly for debugging.

---

## Command Encyclopedia

| Command | Usage | Description |
| :--- | :--- | :--- |
| **GPIO** | `PIN_SET_OUT <PIN>` | Configures a pin as Output |
| | `PIN_HIGH <PIN>` | Sets pin to 3.3V |
| | `PIN_TOGGLE <PIN>` | Flips pin state |
| **UART** | `UART_INIT <1\|6> <BAUD>` | Initialize a UART peripheral |
| | `UART_RS485 <1\|6> <DE_PIN>`| Use a pin for automatic RS485 Drive Enable |
| **I2C** | `I2C_INIT 1 MASTER 100000` | Start I2C1 Master at 100kHz |
| | `I2C_INIT 1 SLAVE 0x68` | Start I2C1 as a Slave at address 0x68 |
| **SPI** | `SPI_INIT 1 MASTER 1000000` | Start SPI1 Master at 1MHz |
| **System** | `STATUS` | View active clocks and peripheral states |
| | `RESET` | Soft-reset the STM32 |

---

## Limitations & Scope
*   **Real-time Constraints**: In Pirate/Bridge mode, the I2C/SPI latency is limited by the Host UART speed (115.2k). It is not suitable for 400kHz+ I2C logic that requires sub-millisecond slave responses unless implemented directly in C.
*   **Voltage**: This is a 3.3V system. **Do not connect 5V logic** directly to PA9/PA10 or other non-5V tolerant pins.
*   **Buffer Size**: Command arguments are limited to 64 bytes per transaction.

---

## Setup & Contribution
1. **STM32 Preparation**: Flash the binary in `firmware/` using `make flash`.
2. **Platform Permissions**: On Linux, ensure your user is in the `dialout` group to avoid `sudo`.

**Developed by Karthik Sarvan**
*Mimic v1.1.0 - 2026*
