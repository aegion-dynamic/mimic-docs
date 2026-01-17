# SPI Troubleshooting Guide

## Issue 1: Clock Speed Fixed ✅

**Problem**: STM32 was running at 50MHz instead of 1MHz
**Cause**: Prescaler calculation logic was inverted
**Fix**: Corrected prescaler selection algorithm
**Action**: Reflash the firmware with `make flash`

---

## Issue 2: Data Not Received by ESP32

**Symptoms**:
- ESP32 shows `RX: 00 00` instead of `AA BB CC DD`
- Only 2 bytes transferred instead of 4

**Possible Causes**:

### 1. Timing Issue (Most Likely)
The ESP32 SPI slave might not be ready when STM32 starts transfer.

**Solution**: Add a small delay after asserting CS:
```bash
SPI_CS A4 LOW
# Wait a moment for ESP32 to be ready
SPI_TRANSFER 1 AA BB CC DD  
SPI_CS A4 HIGH
```

### 2. ESP32 Not in Correct Mode
Ensure ESP32 is in echo mode and Mode 0.

**ESP32 Serial Monitor Commands**:
```
0    # Set to Mode 0
e    # Enable echo mode
```

### 3. Wiring Issues

**Check these connections**:
```
STM32 PA5 (SCK)  → ESP32 GPIO 18
STM32 PA6 (MISO) → ESP32 GPIO 19  ⚠️ CRITICAL
STM32 PA7 (MOSI) → ESP32 GPIO 23
STM32 PA4 (CS)   → ESP32 GPIO 5
STM32 GND        → ESP32 GND
```

**Common mistakes**:
- MISO/MOSI swapped
- Missing GND connection
- Loose connections

**Test with multimeter**:
- Continuity test each wire
- Verify GND is connected

### 4. ESP32 Sketch Issue

The advanced sketch uses hardware SPI which might have initialization issues.

**Try the simple sketch instead**:
Upload `ESP32_SPI_Test.ino` which uses software SPI and is more forgiving.

---

## Issue 3: Only 2 Bytes Transferred

**Cause**: ESP32 might be processing data slower than STM32 sends it.

**Solutions**:

### Option A: Use Simple Sketch
`ESP32_SPI_Test.ino` is more reliable for testing.

### Option B: Lower Speed
Try slower SPI speed:
```bash
SPI_INIT 1 MASTER 100000 0 0   # 100kHz
```

### Option C: Add Delays
```bash
SPI_CS A4 LOW
# Small delay here helps
SPI_TRANSFER 1 AA BB CC DD
# Small delay before deassert
SPI_CS A4 HIGH
```

---

## Recommended Testing Sequence

### Step 1: Verify Wiring
```bash
# In STM32 terminal:
PIN_STATUS A5
PIN_STATUS A6  
PIN_STATUS A7
PIN_STATUS A4
```

### Step 2: Reflash STM32 (IMPORTANT!)
```bash
cd /home/karthik/Documents/Aegion/Mimic/Mimic
make flash
```

### Step 3: Reset ESP32
Press the reset button on ESP32, verify it shows:
```
=== ESP32 Advanced SPI Slave ===
SPI Slave Mode 0 initialized
  CPOL=0, CPHA=0
```

### Step 4: Set ESP32 Mode
In ESP32 serial monitor:
```
0    # Mode 0
e    # Echo mode
```

### Step 5: Test at Low Speed
In STM32 terminal:
```bash
SPI_INIT 1 MASTER 100000 0 0
SPI_STATUS
```

Should now show:
```
Speed: 781250 Hz (requested: 100000 Hz)
```
(Not 50MHz!)

### Step 6: Test Transfer
```bash
SPI_CS A4 LOW
SPI_TRANSFER 1 AA BB CC DD
SPI_CS A4 HIGH
```

**Expected ESP32 output**:
```
Transfer #1: 4 bytes
  RX: AA BB CC DD
  TX: AA BB CC DD
```

**Expected STM32 output**:
```
OK: Transfer complete (4 bytes)
TX: AA BB CC DD
RX: AA BB CC DD
```

---

## If Still Not Working

### Use Loopback Test First
Disconnect ESP32 and connect:
- PA7 (MOSI) → PA6 (MISO)

```bash
SPI_INIT 1 MASTER 1000000 0 0
SPI_TRANSFER 1 DE AD BE EF
```

Should receive back: `DE AD BE EF`

This verifies STM32 SPI is working correctly.

### Try Simple ESP32 Sketch
Upload `ESP32_SPI_Test.ino` instead of the advanced version.

### Check ESP32 Serial Output
Make sure ESP32 is actually running and responding to commands.

---

## Quick Reference

### STM32 Commands
```bash
# Initialize (now with correct speed!)
SPI_INIT 1 MASTER 1000000 0 0

# Check status
SPI_STATUS

# Transfer
SPI_CS A4 LOW
SPI_TRANSFER 1 AA BB CC DD
SPI_CS A4 HIGH
```

### ESP32 Commands  
```bash
0    # Mode 0
1    # Mode 1
2    # Mode 2
3    # Mode 3
e    # Echo mode
c    # Counter mode
s    # Show stats
h    # Help
```
