# STM32 + Arduino UART Testing Setup

## Overview

You now have two approaches:

### Approach 1: Via OpenOCD/GDB (Current - No Firmware Needed)
- Use Mimic.py to control STM32 GPIO via GDB
- Good for debugging
- Requires OpenOCD running

### Approach 2: Program STM32 with Firmware (New - Recommended)
- Flash `stm32_uart_firmware.c` to STM32
- Talk to STM32 via USB serial like Arduino
- Simple one-command setup
- No need for OpenOCD running constantly

---

## Quick Start (Approach 2 - Recommended)

### Step 1: Build STM32 Firmware

```bash
cd ~/Documents/Aegion/Mimic
chmod +x build_stm32.sh
./build_stm32.sh
```

This creates:
- `stm32_uart_firmware.elf` - Main file
- `stm32_uart_firmware.bin` - Binary for flashing
- `stm32_uart_firmware.hex` - Intel HEX format

### Step 2: Flash to STM32

**Option A: Using ST-LINK (ST flash tool)**
```bash
st-flash write stm32_uart_firmware.bin 0x8000000
```

**Option B: Using OpenOCD + GDB**
```bash
# Terminal 1
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

# Terminal 2
arm-none-eabi-gdb stm32_uart_firmware.elf
(gdb) target extended-remote :3333
(gdb) file stm32_uart_firmware.elf
(gdb) load
(gdb) cont
(gdb) quit
```

### Step 3: Get Micro USB Adapter

You need a USB-to-Serial adapter (CP2102, CH340, FTDI) to communicate with STM32 via USB.

Connection:
```
STM32 PA9  (TX) → USB adapter RX
STM32 PA10 (RX) → USB adapter TX
STM32 GND       → USB adapter GND
```

### Step 4: Test Communication

Once you have the micro USB adapter:

```bash
# Find which port STM32 is on
ls -la /dev/ttyUSB*

# Interactive test
python3 uart_tester.py --stm32 /dev/ttyUSB0 --interactive

# Or automated test
python3 uart_tester.py --stm32 /dev/ttyUSB0 --test
```

---

## Arduino Setup (No Changes Needed)

Arduino is already programmed and works:
```bash
python3 uart_tester.py --arduino /dev/ttyUSB0 --test
```

---

## Testing Both Together

Once both have USB adapters:
```bash
python3 uart_tester.py --arduino /dev/ttyUSB0 --stm32 /dev/ttyUSB1 --test
```

---

## Current Limitation

**You need a micro USB adapter for STM32 serial communication.**

The STM32F411 Discovery board has ST-LINK for debugging but not standard USB-serial.

Options:
1. Get USB-to-Serial adapter (cheap, ~$2-5)
2. Use OpenOCD + GDB approach (what you're doing now)
3. Add USB functionality via firmware (complex)

---

## Files Created

```
stm32_uart_firmware.c      - STM32 firmware source
stm32_uart_firmware.elf    - Compiled ELF (after build)
stm32_uart_firmware.bin    - Binary file (after build)
stm32_uart_firmware.hex    - Hex file (after build)
build_stm32.sh             - Build script
uart_tester.py             - Simple serial testing tool
```

---

## When You Get Micro USB

1. Connect STM32 PA9 → USB RX, PA10 → USB TX, GND → GND
2. Plug USB into computer
3. Run: `python3 uart_tester.py --stm32 /dev/ttyUSB1 --interactive`
4. Done!

---

## For Now

You can still test with Mimic.py:
```bash
# Start OpenOCD
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg &

# Use Mimic
python3 Mimic.py
stm32> gpio_out A 9
stm32> gpio_on A 9
stm32> exit
```

Or use Approach 2 when you get a USB adapter!
