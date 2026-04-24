# 📜 Command Encyclopedia

This document lists every supported command in Mimic v1. All commands are case-insensitive and must end with `\r\n`.

---

## 🟢 GPIO (General Purpose I/O)

### `PIN_SET_OUT <PIN>`
Configures the specified pin as a Push-Pull Output.
*   **Example**: `PIN_SET_OUT A5`
*   **Response**: `OK: Configured A5 as OUTPUT`

### `PIN_SET_IN <PIN> [PULL]`
Configures the specified pin as an Input.
*   **PULL Options**: `UP`, `DOWN`, `NONE` (default: `NONE`)
*   **Example**: `PIN_SET_IN B1 UP`

### `PIN_HIGH <PIN>` / `PIN_LOW <PIN>` / `PIN_TOGGLE <PIN>`
Controls the digital state of the pin.
*   **Example**: `PIN_HIGH C13` (Turns off the onboard LED on most BlackPills)

### `PIN_READ <PIN>`
Returns the current digital state (0 or 1).

---

## 📡 UART (Universal Asynchronous RX/TX)

### `UART_INIT <INSTANCE> <BAUD>`
Initializes a UART peripheral. Support instances: `1` (usually host), `2`, `6`.
*   **Example**: `UART_INIT 6 9600`

### `UART_RS485 <INSTANCE> <DE_PIN>`
Enables automatic Data Enable (DE) management for RS485 transceivers. 
*   The `DE_PIN` will go HIGH during transmission and LOW immediately after the last bit is sent.
*   **Example**: `UART_RS485 6 A15`

### `UART_SEND <INSTANCE> <DATA>`
Sends a string of data.

---

## 🔗 I2C (Inter-Integrated Circuit)

### `I2C_INIT <INSTANCE> <MODE> <SPEED|ADDR>`
*   **MASTER Mode**: `I2C_INIT 1 MASTER 100000` (init at 100kHz)
*   **SLAVE Mode**: `I2C_INIT 1 SLAVE 0x68` (init as slave at 0x68)

### `I2C_WRITE <INSTANCE> <ADDR> <HEX_DATA>`
*   **Example**: `I2C_WRITE 1 0x50 00 A1 B2`

### `I2C_READ <INSTANCE> <ADDR> <LEN>`
*   Reads `<LEN>` bytes from the master or slave.

---

## ⚙️ System Commands

### `STATUS`
Displays a technical summary of the system, including:
*   Active GPIO clocks.
*   Peripherals currently initialized.
*   Core clock frequency.

### `RESET`
Performs a software reset of the STM32 (`NVIC_SystemReset`). Useful for starting fresh without unplugging the USB.
