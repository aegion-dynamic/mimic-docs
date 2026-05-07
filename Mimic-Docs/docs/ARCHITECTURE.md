# 🏛 Architecture Overview

Mimic v1 is built on a **Command-Response Bridge** architecture. It transforms complex, low-level hardware timings into human-readable string transactions.

## 🔄 The Data Flow

1.  **Host Request**: The PC (Python) sends a UTF-8 string over UART (e.g., `PIN_HIGH A5\r\n`).
2.  **Firmware Parsing**: The STM32 receives this via an interrupt-driven buffer. It tokensizes the command and arguments.
3.  **Action Execution**: The internal `mimic.c` engine calls the relevant HAL function (e.g., `HAL_GPIO_WritePin`).
4.  **Hardware Response**: The STM32 captures the result (Success, Data, or Error Code).
5.  **Host Response**: The STM32 sends back a structured response string (e.g., `OK: Pin A5 set HIGH\r\n> `).

## 🛠 The Two Layers

### Layer 1: The C-Engine (Firmware)
*   **Interrupt Managed**: Keyboard input is handled via `USART1_IRQHandler`.
*   **Blocking Transactions**: Peripheral actions (I2C/SPI) are performed in blocking mode to ensure data integrity before responding to the PC.
*   **Stateless logic**: The firmware maintains the status of initialized peripherals but treats every GPIO command as an atomic operation.

### Layer 2: The Python-Shell (MimicLib)
*   **Auto-Discovery**: Scans all `COM`/`tty` ports, sending a `VERSION` challenge to identify the Mimic signature.
*   **Command Serialization**: Converts high-level Python method calls into the exact string syntax required by the C-Engine.
*   **Abstraction Layer**: Provides specialized classes like `MPU6050Simulator` which act as state machines on the PC side, reacting to hardware events reported by the STM32.

---

## ⚡ Real-time Performance
While the STM32 can toggle pins in nanoseconds, the bottleneck is the **115200 Baud link**. Commands take approx **1-2ms** to transmit and process. For high-speed logic, the "Sensor Mocking" mode is preferred, as it allows the STM32 to stretch the I2C clock and wait for the Python handler to respond.
