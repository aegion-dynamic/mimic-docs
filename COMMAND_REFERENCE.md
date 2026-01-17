# MIMIC - Complete Command Reference

**STM32F411 Discovery Board Command Interface**  
Version 1.1.0

---

## Table of Contents

1. [GPIO Commands](#gpio-commands)
2. [UART Commands](#uart-commands)
3. [SPI Commands](#spi-commands)
4. [System Commands](#system-commands)
5. [Pin Mappings](#pin-mappings)
6. [Quick Reference](#quick-reference)

---

## GPIO Commands

### PIN_STATUS
**Show pin configuration and current state**

```
Syntax: PIN_STATUS <PIN>
Shortcut: PS
```

**Examples:**
```bash
PIN_STATUS A5      # Check PA5 status
PIN_STATUS D12     # Check PD12 status
PS B3              # Using shortcut
```

**Response:**
```
Pin: PA5
Mode: OUTPUT
State: HIGH
Pull: NONE
Speed: MEDIUM
```

---

### PIN_SET_OUT
**Configure pin as output**

```
Syntax: PIN_SET_OUT <PIN>
Shortcut: PSO
```

**Examples:**
```bash
PIN_SET_OUT A5     # Set PA5 as output
PIN_SET_OUT LED    # Set LED pin as output
PSO D12            # Using shortcut
```

---

### PIN_SET_IN
**Configure pin as input with optional pull resistor**

```
Syntax: PIN_SET_IN <PIN> [PULL]
Shortcut: PSI

PULL options: UP, DOWN, NONE (default: NONE)
```

**Examples:**
```bash
PIN_SET_IN A5           # Input with no pull
PIN_SET_IN B3 UP        # Input with pull-up
PIN_SET_IN C7 DOWN      # Input with pull-down
PSI BUTTON NONE         # Using shortcut
```

---

### PIN_HIGH
**Set output pin to HIGH (3.3V)**

```
Syntax: PIN_HIGH <PIN>
Shortcut: PH
```

**Examples:**
```bash
PIN_HIGH A5        # Set PA5 HIGH
PIN_HIGH LED       # Turn on LED
PH D12             # Using shortcut
```

---

### PIN_LOW
**Set output pin to LOW (0V)**

```
Syntax: PIN_LOW <PIN>
Shortcut: PL
```

**Examples:**
```bash
PIN_LOW A5         # Set PA5 LOW
PIN_LOW LED        # Turn off LED
PL D12             # Using shortcut
```

---

### PIN_READ
**Read current state of pin**

```
Syntax: PIN_READ <PIN>
Shortcut: PR
```

**Examples:**
```bash
PIN_READ A5        # Read PA5 state
PIN_READ BUTTON    # Read button state
PR B3              # Using shortcut
```

**Response:**
```
Pin A5: HIGH
```

---

### PIN_TOGGLE
**Toggle output pin state (HIGH ↔ LOW)**

```
Syntax: PIN_TOGGLE <PIN>
Shortcut: PT
```

**Examples:**
```bash
PIN_TOGGLE A5      # Toggle PA5
PIN_TOGGLE LED     # Toggle LED
PT D12             # Using shortcut
```

---

### PIN_MODE
**Set pin mode**

```
Syntax: PIN_MODE <PIN> <MODE>
Shortcut: PM

MODE options: IN, OUT, AF, AN
```

**Examples:**
```bash
PIN_MODE A5 OUT    # Output mode
PIN_MODE B3 IN     # Input mode
PIN_MODE C7 AF     # Alternate function
PIN_MODE A0 AN     # Analog mode
```

---

## UART Commands

### UART_INIT
**Initialize UART peripheral**

```
Syntax: UART_INIT <1|6> <BAUD> [PARITY] [STOP]
Shortcut: UI

PARITY: N (None), E (Even), O (Odd) - default: N
STOP: 1, 2 - default: 1
```

**Available UARTs:**
- UART1: PA9 (TX), PA10 (RX)
- UART6: PC6 (TX), PC7 (RX)
- UART2: Reserved for host communication

**Examples:**
```bash
UART_INIT 1 9600              # 9600 baud, no parity, 1 stop bit
UART_INIT 6 115200            # 115200 baud
UART_INIT 1 57600 E 2         # Even parity, 2 stop bits
UI 1 9600                     # Using shortcut
```

**Response:**
```
OK: UART1 initialized
Baudrate: 9600
Parity: None
Stop bits: 1
```

---

### UART_SEND
**Send data via UART**

```
Syntax: UART_SEND <1|6> <DATA>
Shortcut: US
```

**Examples:**
```bash
UART_SEND 1 Hello World       # Send text
UART_SEND 6 "Test Data"       # Send with quotes
US 1 AT+CMD                   # Using shortcut
```

**Escape Sequences:**
- `\r` - Carriage return
- `\n` - Newline
- `\t` - Tab
- `\\` - Backslash

---

### UART_RECV
**Receive data from UART**

```
Syntax: UART_RECV <1|6> <LENGTH> [TIMEOUT_MS]
Shortcut: UR

Default timeout: 1000ms
```

**Examples:**
```bash
UART_RECV 1 10                # Receive 10 bytes
UART_RECV 6 20 500            # Receive 20 bytes, 500ms timeout
UR 1 5                        # Using shortcut
```

**Response:**
```
OK: Received 10 bytes
Data: Hello World
```

---

### UART_STATUS
**Show status of all UART peripherals**

```
Syntax: UART_STATUS
```

**Example:**
```bash
UART_STATUS
```

**Response:**
```
UART1: Initialized, 9600 baud
UART6: Not initialized
UART2: Host interface, 9600 baud
```

---

## SPI Commands

### SPI_INIT
**Initialize SPI peripheral with automatic CS control**

```
Syntax: SPI_INIT <1-5> <MASTER|SLAVE> <SPEED> [CPOL] [CPHA] [SIZE] [ORDER] [CS_PIN]
Shortcut: SI

CPOL: 0 (low), 1 (high) - default: 0
CPHA: 0 (1st edge), 1 (2nd edge) - default: 0
SIZE: 8, 16 - default: 8
ORDER: MSB, LSB - default: MSB
CS_PIN: GPIO pin for automatic CS control (e.g., A4, B12)
```

**SPI Modes:**
- Mode 0: CPOL=0, CPHA=0
- Mode 1: CPOL=0, CPHA=1
- Mode 2: CPOL=1, CPHA=0
- Mode 3: CPOL=1, CPHA=1

**Examples:**
```bash
# Basic initialization
SPI_INIT 1 MASTER 1000000

# With automatic CS control
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4

# SPI Mode 3, 16-bit, LSB first
SPI_INIT 2 MASTER 500000 1 1 16 LSB B12

# Using shortcut
SI 1 MASTER 1000000 0 0 8 MSB A4
```

**Response:**
```
OK: SPI1 initialized
  Mode: MASTER
  Speed: 781250 Hz (requested: 1000000 Hz)
  CPOL: 0, CPHA: 0
  Data size: 8-bit, Bit order: MSB
  CS: A4 (automatic control enabled)
```

---

### SPI_SEND
**Send data via SPI (half-duplex TX)**

```
Syntax: SPI_SEND <1-5> <HEX_DATA>
Shortcut: SS

Note: CS is automatically controlled if configured in SPI_INIT
```

**Examples:**
```bash
SPI_SEND 1 AA BB CC DD        # Send 4 bytes
SPI_SEND 2 01 02 03           # Send 3 bytes
SS 1 FF EE DD CC              # Using shortcut
```

**Response:**
```
OK: Sent 4 bytes: AA BB CC DD
```

---

### SPI_RECV
**Receive data from SPI (half-duplex RX)**

```
Syntax: SPI_RECV <1-5> <LENGTH> [TIMEOUT_MS]
Shortcut: SR

Default timeout: 1000ms
Note: CS is automatically controlled if configured in SPI_INIT
```

**Examples:**
```bash
SPI_RECV 1 4                  # Receive 4 bytes
SPI_RECV 2 8 500              # Receive 8 bytes, 500ms timeout
SR 1 10                       # Using shortcut
```

**Response:**
```
OK: Received 4 bytes: 01 02 03 04
```

---

### SPI_TRANSFER
**Full-duplex SPI transfer (simultaneous TX and RX)**

```
Syntax: SPI_TRANSFER <1-5> <HEX_DATA>
Shortcut: ST

Note: CS is automatically controlled if configured in SPI_INIT
```

**Examples:**
```bash
SPI_TRANSFER 1 AA BB CC DD    # Send and receive 4 bytes
SPI_TRANSFER 2 FF EE DD CC    # Send and receive 4 bytes
ST 1 01 02 03 04              # Using shortcut
```

**Response:**
```
OK: Transfer complete (4 bytes)
  TX: AA BB CC DD
  RX: 12 34 56 78
```

---

### SPI_CS
**Manually control chip select pin**

```
Syntax: SPI_CS <PIN> <HIGH|LOW>
Shortcut: SCS
```

**Examples:**
```bash
SPI_CS A4 LOW                 # Assert CS (active low)
SPI_CS A4 HIGH                # Deassert CS
SCS B12 LOW                   # Using shortcut
```

**Note:** Only needed if CS pin not configured in SPI_INIT

---

### SPI_STATUS
**Show status of all SPI peripherals**

```
Syntax: SPI_STATUS
```

**Example:**
```bash
SPI_STATUS
```

**Response:**
```
SPI1: Initialized, MASTER, 781250 Hz, Mode 0
SPI2: Not initialized
SPI3: Not initialized
SPI4: Not initialized
SPI5: Not initialized
```

---

## I2C Commands

### I2C_INIT
**Initialize I2C peripheral**

```
Syntax: I2C_INIT <1-3> <MASTER|SLAVE> <SPEED|OWN_ADDR> [ADDR_MODE]
Shortcut: II
```

**Parameters:**
- **Mode**:
  - `MASTER`: STM32 controls the bus
  - `SLAVE`: STM32 waits for requests from another Master
- **Value**:
  - If MASTER: Clock speed in Hz (1000-400000)
  - If SLAVE: 7-bit Own Address (e.g., 0x30)
- **Addr Mode** (Optional):
  - `10`: 10-bit addressing
  - Default: 7-bit

**Examples:**
```bash
# Master at 100kHz
I2C_INIT 1 MASTER 100000

# Slave at address 0x30
I2C_INIT 1 SLAVE 0x30

# Master at 400kHz (Fast Mode)
I2C_INIT 2 MASTER 400000

# Using shortcut
II 1 MASTER 100000
```

**Response:**
```
OK: I2C1 initialized as MASTER
  Speed: 100000 Hz
```

---

### I2C_SCAN
**Scan bus for connected devices**

```
Syntax: I2C_SCAN <1-3>
Shortcut: IS
```

**Example:**
```bash
I2C_SCAN 1
```

**Response:**
```
Scanning I2C1 bus...
Found device at 0x3C
Found device at 0x68
Scan complete. Found 2 devices.
```

---

### I2C_WRITE
**Write data to I2C bus**

```
Syntax: I2C_WRITE <1-3> <ADDR> <HEX_DATA>
Shortcut: IW
```

**Behavior:**
- **Master Mode**: Writes `HEX_DATA` to slave at `ADDR`
- **Slave Mode**: `ADDR` is ignored. Sends `HEX_DATA` when Master requests read.

**Examples:**
```bash
# Master: Write to register 0x6B
I2C_WRITE 1 0x68 6B 00

# Slave: Load data buffer for next Master read
I2C_WRITE 1 0x00 AA BB CC
```

---

### I2C_READ
**Read data from I2C bus**

```
Syntax: I2C_READ <1-3> <ADDR> <LENGTH>
Shortcut: IR
```

**Behavior:**
- **Master Mode**: Reads `LENGTH` bytes from slave at `ADDR`
- **Slave Mode**: `ADDR` is ignored. Waits to receive `LENGTH` bytes from Master.

**Examples:**
```bash
# Master: Read 6 bytes
I2C_READ 1 0x68 6

# Slave: Wait to receive 4 bytes
I2C_READ 1 0x00 4
```

---

### I2C_WRITE_READ
**Write command/address then read response (Repeated Start)**

```
Syntax: I2C_WRITE_READ <1-3> <ADDR> <HEX_DATA> <READ_LEN>
Shortcut: IWR
```

**Examples:**
```bash
I2C_WRITE_READ 1 0x68 75 1    # Read WHO_AM_I register (0x75)
I2C_WRITE_READ 2 0x50 00 10   # Read 16 bytes from EEPROM addr 0x00
```

**Response:**
```
OK: Read 1 bytes from 0x68: 68
```

---

### I2C_STATUS
**Show status of all I2C peripherals**

```
Syntax: I2C_STATUS
```

**Example:**
```bash
I2C_STATUS
```

**Response:**
```
I2C1: Initialized 100000 Hz, 7-bit
I2C2: Not initialized
I2C3: Not initialized
```

---

## System Commands

### HELP
**Show command help**

```
Syntax: HELP [COMMAND]
```

**Examples:**
```bash
HELP                          # Show all commands
HELP SPI_INIT                 # Show help for SPI_INIT
HELP GPIO                     # Show GPIO commands
```

---

### VERSION
**Show firmware version**

```
Syntax: VERSION
Shortcut: VER
```

**Example:**
```bash
VERSION
```

**Response:**
```
MIMIC Firmware v1.1.0
Target: STM32F411VET6 Discovery
Built: Jan 17 2026 13:15:42
```

---

### STATUS
**Show system status**

```
Syntax: STATUS
```

**Example:**
```bash
STATUS
```

**Response:**
```
=== System Status ===
Uptime: 12345 ms
Host UART: 9600 baud

GPIO Clocks Enabled:
  GPIOA  GPIOB  GPIOC  GPIOD  GPIOE
```

---

### RESET
**Reset the microcontroller**

```
Syntax: RESET
```

**Example:**
```bash
RESET
```

---

## Pin Mappings

### STM32F411 Discovery Board

**GPIO Ports:**
- Port A: PA0-PA15
- Port B: PB0-PB15
- Port C: PC0-PC15
- Port D: PD0-PD15
- Port E: PE0-PE15

**Special Pins:**
- LED: PD12, PD13, PD14, PD15 (onboard LEDs)
- BUTTON: PA0 (user button)

### UART Pin Mappings

| UART | TX Pin | RX Pin | Notes |
|------|--------|--------|-------|
| UART1 | PA9 | PA10 | Available |
| UART2 | PA2 | PA3 | **Reserved for host** |
| UART6 | PC6 | PC7 | Available |

### SPI Pin Mappings

| SPI | SCK | MISO | MOSI | NSS |
|-----|-----|------|------|-----|
| SPI1 | PA5 | PA6 | PA7 | PA4 |
| SPI2 | PB13 | PB14 | PB15 | PB12 |
| SPI3 | PB3 | PB4 | PB5 | PA15 |
| SPI4 | PE2 | PE5 | PE6 | PE4 |
| SPI5 | PE12 | PE13 | PE14 | PE11 |

---

## Quick Reference

### Command Shortcuts

| Shortcut | Full Command | Description |
|----------|--------------|-------------|
| PS | PIN_STATUS | Show pin status |
| PSO | PIN_SET_OUT | Set pin as output |
| PSI | PIN_SET_IN | Set pin as input |
| PH | PIN_HIGH | Set pin HIGH |
| PL | PIN_LOW | Set pin LOW |
| PR | PIN_READ | Read pin state |
| PT | PIN_TOGGLE | Toggle pin |
| PM | PIN_MODE | Set pin mode |
| UI | UART_INIT | Initialize UART |
| US | UART_SEND | Send via UART |
| UR | UART_RECV | Receive via UART |
| SI | SPI_INIT | Initialize SPI |
| SS | SPI_SEND | Send via SPI |
| SR | SPI_RECV | Receive via SPI |
| ST | SPI_TRANSFER | SPI transfer |
| SCS | SPI_CS | Control CS pin |
| II | I2C_INIT | Initialize I2C |
| IS | I2C_SCAN | Scan I2C bus |
| IW | I2C_WRITE | Write via I2C |
| IR | I2C_READ | Read via I2C |
| IWR | I2C_WRITE_READ | Write then read |
| VER | VERSION | Show version |

### Common Use Cases

#### Blink an LED
```bash
PIN_SET_OUT D12
PIN_TOGGLE D12
```

#### Read a Button
```bash
PIN_SET_IN A0 UP
PIN_READ A0
```

#### UART Communication
```bash
UART_INIT 1 9600
UART_SEND 1 Hello
UART_RECV 1 10
```

#### SPI Communication with Auto CS
```bash
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4
SPI_TRANSFER 1 AA BB CC DD
```

#### SPI Communication Manual CS
```bash
SPI_INIT 1 MASTER 1000000
SPI_CS A4 LOW
SPI_TRANSFER 1 AA BB CC DD
SPI_CS A4 HIGH
```

---

## Tips & Tricks

### Hex Data Format
- Valid hex digits: 0-9, A-F, a-f
- Separate bytes with spaces: `AA BB CC DD`
- Or continuous: `AABBCCDD`
- **Invalid**: `HE LL OO` (H, L, O are not hex digits)

### SPI Modes
- **Mode 0** (most common): CPOL=0, CPHA=0
- **Mode 1**: CPOL=0, CPHA=1
- **Mode 2**: CPOL=1, CPHA=0
- **Mode 3**: CPOL=1, CPHA=1

### Automatic CS Control
When you specify a CS pin in `SPI_INIT`, all SPI commands automatically:
1. Assert CS (LOW) before transfer
2. Perform the transfer
3. Deassert CS (HIGH) after transfer

This eliminates the need for manual `SPI_CS` commands!

### Command History
The enhanced CLI saves command history. Use:
- **↑** (Up arrow) - Previous command
- **↓** (Down arrow) - Next command
- **TAB** - Autocomplete
- **Ctrl+C** - Cancel current line
- **Ctrl+D** or `exit` - Quit

---

## Troubleshooting

### No Response from Device
1. Check serial port: `ls /dev/ttyUSB*`
2. Verify baud rate (default: 9600)
3. Try resetting the board
4. Check USB cable connection

### SPI Not Working
1. Verify wiring matches pin mappings
2. Check SPI mode matches slave device
3. Ensure CS pin is configured correctly
4. Try lower clock speed (100kHz for testing)

### UART Not Working
1. Check TX/RX are not swapped
2. Verify baud rate matches
3. Check parity and stop bit settings
4. Ensure UART is initialized before use

---

**For more information, visit the project repository or type `HELP` in the CLI.**
