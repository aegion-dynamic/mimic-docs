# Automatic CS Control - Quick Guide

## 🎉 New Feature: Automatic CS Control!

No more manual `SPI_CS` commands! Just specify the CS pin once during initialization.

## Usage

### Old Way (Manual CS) ❌
```bash
SPI_INIT 1 MASTER 1000000
SPI_CS A4 LOW
SPI_TRANSFER 1 AA BB CC DD
SPI_CS A4 HIGH
```

### New Way (Automatic CS) ✅
```bash
# Initialize with CS pin
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4

# Transfer automatically controls CS!
SPI_TRANSFER 1 AA BB CC DD
```

## Examples

### Basic Setup with Auto CS
```bash
# Initialize SPI1 with A4 as CS pin
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4

# Now just send data - CS is automatic!
SPI_TRANSFER 1 AA BB CC DD
```

### Receiving Data from ESP32

**ESP32 Setup** (in Serial Monitor):
```
0    # Mode 0
e    # Echo mode
```

**STM32 Commands**:
```bash
# Initialize with automatic CS
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4

# Send and receive - CS handled automatically!
SPI_TRANSFER 1 AA BB CC DD
```

**Expected Output**:
```
OK: Transfer complete (4 bytes)
  TX: AA BB CC DD
  RX: AA BB CC DD    ← ESP32 echoed back!
```

### Different SPI Modes

```bash
# Mode 0 (CPOL=0, CPHA=0)
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4

# Mode 1 (CPOL=0, CPHA=1)
SPI_INIT 1 MASTER 1000000 0 1 8 MSB A4

# Mode 2 (CPOL=1, CPHA=0)
SPI_INIT 1 MASTER 1000000 1 0 8 MSB A4

# Mode 3 (CPOL=1, CPHA=1)
SPI_INIT 1 MASTER 1000000 1 1 8 MSB A4
```

## All Commands with Auto CS

### SPI_SEND
```bash
SPI_SEND 1 DE AD BE EF
# CS goes LOW → sends data → CS goes HIGH
```

### SPI_RECV
```bash
SPI_RECV 1 4
# CS goes LOW → receives 4 bytes → CS goes HIGH
```

### SPI_TRANSFER
```bash
SPI_TRANSFER 1 CA FE BA BE
# CS goes LOW → sends & receives → CS goes HIGH
```

## Manual CS Still Available

If you don't specify a CS pin during init, you can still control it manually:

```bash
# Initialize without CS pin
SPI_INIT 1 MASTER 1000000

# Manual control
SPI_CS A4 LOW
SPI_TRANSFER 1 AA BB CC DD
SPI_CS A4 HIGH
```

## Complete Test Sequence

```bash
# 1. Initialize with auto CS
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4

# 2. Check status
SPI_STATUS

# 3. Send test data
SPI_TRANSFER 1 AA BB CC DD

# 4. Send more data
SPI_TRANSFER 1 11 22 33 44

# 5. Receive from ESP32 (if in counter mode)
SPI_RECV 1 8
```

## Troubleshooting

### ESP32 Not Receiving Data?

1. **Check ESP32 is in echo mode**:
   ```
   e    # In ESP32 serial monitor
   ```

2. **Verify wiring**:
   - STM32 PA5 → ESP32 GPIO18 (SCK)
   - STM32 PA6 → ESP32 GPIO19 (MISO)
   - STM32 PA7 → ESP32 GPIO23 (MOSI)
   - STM32 PA4 → ESP32 GPIO5 (CS)
   - GND → GND

3. **Check SPI mode matches**:
   - Both should be in Mode 0

### Not Receiving Echo Back?

ESP32 needs to be in echo mode. In ESP32 serial monitor:
```
0    # Set Mode 0
e    # Enable echo
```

Then try again:
```bash
SPI_TRANSFER 1 AA BB CC DD
```

You should see:
- **STM32 RX**: `AA BB CC DD`
- **ESP32 RX**: `AA BB CC DD`
