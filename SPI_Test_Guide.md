# ESP32 ↔ STM32 SPI Testing Guide

Complete guide for testing your STM32F411 Mimic SPI implementation using an ESP32.

## Hardware Setup

### Pin Connections

| STM32F411 (SPI1) | ESP32 (VSPI) | Wire Color Suggestion |
|------------------|--------------|----------------------|
| PA5 (SCK)        | GPIO 18      | Yellow               |
| PA6 (MISO)       | GPIO 19      | Blue                 |
| PA7 (MOSI)       | GPIO 23      | Green                |
| PA4 (CS)         | GPIO 5       | Orange               |
| GND              | GND          | Black                |

**IMPORTANT**: 
- Connect GND between both boards
- ESP32 runs at 3.3V - STM32F411 GPIO is 5V tolerant but outputs 3.3V, so this is safe
- Keep wires short (< 20cm) for reliable communication

### Wiring Diagram

```
STM32F411 Discovery          ESP32
┌─────────────┐          ┌──────────┐
│             │          │          │
│  PA5 (SCK)  ├─────────►│ GPIO 18  │
│  PA6 (MISO) │◄─────────┤ GPIO 19  │
│  PA7 (MOSI) ├─────────►│ GPIO 23  │
│  PA4 (CS)   ├─────────►│ GPIO 5   │
│  GND        ├──────────┤ GND      │
│             │          │          │
└─────────────┘          └──────────┘
```

## Software Setup

### 1. Flash ESP32

Upload one of the provided sketches to your ESP32:
- **ESP32_SPI_Test.ino** - Simple software-based SPI slave
- **ESP32_SPI_Advanced.ino** - Hardware SPI slave with DMA (recommended)

### 2. Flash STM32

Flash your Mimic firmware to the STM32F411:
```bash
cd /home/karthik/Documents/Aegion/Mimic/Mimic
make flash
```

### 3. Connect to Both Devices

**Terminal 1** - ESP32 Serial Monitor:
```bash
# Find ESP32 port
ls /dev/ttyUSB*

# Connect (adjust port as needed)
screen /dev/ttyUSB1 115200
# or
arduino-cli monitor -p /dev/ttyUSB1 -c baudrate=115200
```

**Terminal 2** - STM32 Mimic Interface:
```bash
cd /home/karthik/Documents/Aegion/Mimic
python3 Mimic.py --port /dev/ttyUSB0 --baud 9600
```

## Testing Procedures

### Test 1: SPI Mode 0 (CPOL=0, CPHA=0)

**ESP32 Terminal:**
```
0          # Set ESP32 to Mode 0
e          # Enable echo mode
```

**STM32 Terminal:**
```
SPI_INIT 1 MASTER 1000000 0 0
SPI_CS A4 LOW
SPI_TRANSFER 1 AA BB CC DD
SPI_CS A4 HIGH
```

**Expected Result:**
- ESP32 shows: `RX: AA BB CC DD`
- STM32 shows: `TX: AA BB CC DD | RX: AA BB CC DD` (echo)

---

### Test 2: SPI Mode 1 (CPOL=0, CPHA=1)

**ESP32 Terminal:**
```
1          # Set ESP32 to Mode 1
```

**STM32 Terminal:**
```
SPI_INIT 1 MASTER 1000000 0 1
SPI_CS A4 LOW
SPI_TRANSFER 1 11 22 33 44
SPI_CS A4 HIGH
```

---

### Test 3: SPI Mode 2 (CPOL=1, CPHA=0)

**ESP32 Terminal:**
```
2          # Set ESP32 to Mode 2
```

**STM32 Terminal:**
```
SPI_INIT 1 MASTER 1000000 1 0
SPI_CS A4 LOW
SPI_TRANSFER 1 55 66 77 88
SPI_CS A4 HIGH
```

---

### Test 4: SPI Mode 3 (CPOL=1, CPHA=1)

**ESP32 Terminal:**
```
3          # Set ESP32 to Mode 3
```

**STM32 Terminal:**
```
SPI_INIT 1 MASTER 1000000 1 1
SPI_CS A4 LOW
SPI_TRANSFER 1 99 AA BB CC
SPI_CS A4 HIGH
```

---

### Test 5: Counter Mode

**ESP32 Terminal:**
```
0          # Mode 0
c          # Counter mode (ESP32 sends incrementing values)
```

**STM32 Terminal:**
```
SPI_INIT 1 MASTER 1000000 0 0
SPI_CS A4 LOW
SPI_RECV 1 10
SPI_CS A4 HIGH
```

**Expected Result:**
- STM32 receives: `00 01 02 03 04 05 06 07 08 09`

---

### Test 6: Pattern Mode (Advanced sketch only)

**ESP32 Terminal:**
```
p          # Pattern mode (0x55/0xAA alternating)
```

**STM32 Terminal:**
```
SPI_CS A4 LOW
SPI_RECV 1 8
SPI_CS A4 HIGH
```

**Expected Result:**
- STM32 receives: `55 AA 55 AA 55 AA 55 AA`

---

### Test 7: Different Speeds

Test various clock speeds:

```bash
# 100 kHz
SPI_INIT 1 MASTER 100000 0 0

# 500 kHz
SPI_INIT 1 MASTER 500000 0 0

# 1 MHz
SPI_INIT 1 MASTER 1000000 0 0

# 2 MHz
SPI_INIT 1 MASTER 2000000 0 0

# 5 MHz
SPI_INIT 1 MASTER 5000000 0 0

# 10 MHz
SPI_INIT 1 MASTER 10000000 0 0
```

Then send data:
```
SPI_CS A4 LOW
SPI_TRANSFER 1 DE AD BE EF
SPI_CS A4 HIGH
```

---

### Test 8: Burst Transfer

Send multiple bytes continuously:

```bash
SPI_INIT 1 MASTER 1000000 0 0

# Send 16 bytes
SPI_CS A4 LOW
SPI_TRANSFER 1 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
SPI_CS A4 HIGH
```

---

### Test 9: Half-Duplex TX Only

**ESP32 Terminal:**
```
e          # Echo mode
```

**STM32 Terminal:**
```
SPI_INIT 1 MASTER 1000000 0 0
SPI_CS A4 LOW
SPI_SEND 1 CA FE BA BE
SPI_CS A4 HIGH
```

**Expected Result:**
- ESP32 receives: `CA FE BA BE`

---

### Test 10: Half-Duplex RX Only

**ESP32 Terminal:**
```
c          # Counter mode
```

**STM32 Terminal:**
```
SPI_CS A4 LOW
SPI_RECV 1 8 1000
SPI_CS A4 HIGH
```

**Expected Result:**
- STM32 receives incrementing counter values

---

## Troubleshooting

### No Communication

1. **Check wiring** - Verify all connections with multimeter
2. **Check GND** - Must be connected between boards
3. **Check CS polarity** - Should be LOW during transfer
4. **Verify SPI modes match** - Both devices must use same mode

### Garbled Data

1. **Try lower speed** - Start with 100kHz
2. **Check wire length** - Keep under 20cm
3. **Add ground wire** - Use twisted pair for SCK/MOSI
4. **Check mode** - CPOL/CPHA must match exactly

### Intermittent Errors

1. **Add delays** - Small delay between CS and transfer
2. **Check power** - Ensure stable 3.3V on ESP32
3. **Reduce speed** - Lower clock frequency
4. **Check for noise** - Keep away from power cables

## Quick Reference Commands

### STM32 Commands

```bash
# Initialize SPI
SPI_INIT <1-5> <MASTER|SLAVE> <SPEED> [CPOL] [CPHA] [SIZE] [ORDER]

# Control CS
SPI_CS <PIN> <HIGH|LOW>

# Send data
SPI_SEND <1-5> <HEX_DATA>

# Receive data
SPI_RECV <1-5> <LENGTH> [TIMEOUT]

# Full-duplex
SPI_TRANSFER <1-5> <HEX_DATA>

# Status
SPI_STATUS
```

### ESP32 Commands

```bash
# Mode selection
0, 1, 2, 3     # SPI Mode

# Test modes
e              # Echo
c              # Counter
p              # Pattern (advanced only)
r              # Random (advanced only)

# Utilities
s              # Statistics
x              # Reset
h              # Help
```

## Success Criteria

✅ All 4 SPI modes work correctly
✅ Data echoes back accurately in echo mode
✅ Counter increments properly in counter mode
✅ No data corruption at speeds up to 5 MHz
✅ CS control works reliably
✅ Both half-duplex and full-duplex work

## Next Steps

Once basic SPI is working:
1. Test with actual SPI devices (sensors, displays, etc.)
2. Implement I2C support using similar pattern
3. Add DMA support for high-speed transfers
4. Create automated test scripts
