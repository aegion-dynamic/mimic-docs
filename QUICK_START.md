# MIMIC - Quick Start Guide

Get started with MIMIC in 5 minutes!

## Installation

### 1. Install Python Dependencies
```bash
pip3 install prompt_toolkit pyserial --user
```

### 2. Connect Your STM32
- Connect STM32F411 Discovery board via USB
- Note the serial port (e.g., `/dev/ttyUSB0` on Linux, `COM3` on Windows)

### 3. Run Enhanced CLI
```bash
cd /home/karthik/Documents/Aegion/Mimic
python3 Mimic_Enhanced.py --port /dev/ttyUSB0
```

## First Commands

### Blink the Onboard LED
```bash
mimic> PIN_SET_OUT D12
mimic> PIN_HIGH D12
mimic> PIN_LOW D12
mimic> PIN_TOGGLE D12
```

### Test UART
```bash
mimic> UART_INIT 1 9600
mimic> UART_SEND 1 Hello World
```

### Test SPI with Auto CS
```bash
mimic> SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4
mimic> SPI_TRANSFER 1 AA BB CC DD
```

## Features

### Autocomplete
Press **TAB** to see available commands and get suggestions.

### Syntax Hints
As you type, the bottom toolbar shows:
- Command syntax
- Example usage

### Command History
- **↑/↓** arrows to navigate history
- History is saved between sessions

### Shortcuts
Use shortcuts for faster typing:
```bash
PS A5      # Instead of PIN_STATUS A5
PH D12     # Instead of PIN_HIGH D12
SI 1 ...   # Instead of SPI_INIT 1 ...
```

## Common Tasks

### GPIO Control
```bash
# Set pin as output and toggle
PIN_SET_OUT A5
PIN_TOGGLE A5

# Read input with pull-up
PIN_SET_IN B3 UP
PIN_READ B3
```

### UART Communication
```bash
# Initialize and send
UART_INIT 1 115200
UART_SEND 1 AT+CMD
UART_RECV 1 10
```

### SPI Communication
```bash
# With automatic CS control
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4
SPI_TRANSFER 1 DE AD BE EF

# Check status
SPI_STATUS
```

## Help

### In-CLI Help
```bash
mimic> help              # Show all commands
mimic> help SPI_INIT     # Show specific command help
```

### Documentation
- **COMMAND_REFERENCE.md** - Complete command reference
- **Auto_CS_Guide.md** - SPI automatic CS control guide
- **SPI_Test_Guide.md** - SPI testing with ESP32

## Tips

1. **Use TAB** - Autocomplete makes typing faster
2. **Check syntax** - Bottom toolbar shows correct syntax
3. **Use shortcuts** - Save time with command shortcuts
4. **View examples** - Type `help <command>` for examples
5. **Save history** - Your commands are saved automatically

## Troubleshooting

### Can't connect?
```bash
# List available ports
ls /dev/ttyUSB*

# Try different port
python3 Mimic_Enhanced.py --port /dev/ttyUSB1
```

### Wrong baud rate?
```bash
python3 Mimic_Enhanced.py --port /dev/ttyUSB0 --baud 115200
```

### Need help?
```bash
mimic> help
mimic> version
mimic> status
```

---

**Ready to go! Type your first command and press TAB to explore!**
