# I2C Testing with ESP32 Guide

This guide details how to test the STM32 I2C implementation using an ESP32 as a slave device.

## 1. Hardware Setup

### Wiring
Connect the STM32F411 (Master) to ESP32 (Slave).

| Signal | STM32 (I2C1) | ESP32 | Notes |
|--------|--------------|-------|-------|
| SCL    | PB6          | GPIO 22 | I2C Clock |
| SDA    | PB7          | GPIO 21 | I2C Data  |
| GND    | GND          | GND   | **Common Ground Required** |

> **Note:** Both boards use 3.3V logic, so no level shifter is needed.
> The firmware enables internal pull-ups, but for best reliability (especially at high speeds), you can add external 4.7kΩ resistors from SDA/SCL to 3.3V.

### ESP32 Setup
1. Open the [ESP32_I2C_Slave.ino](ESP32_I2C_Slave.ino) file.
2. Select your ESP32 board in Arduino IDE.
3. Upload the sketch.
4. Open Serial Monitor at **115200 baud**.
5. You should see:
   ```
   === ESP32 I2C Slave ===
   I2C Address: 0x55
   SDA: GPIO 21
   SCL: GPIO 22
   Ready! Waiting for I2C commands...
   ```

---

## 2. Testing Procedure

Connect via the Mimic CLI (`python3 Mimic_Enhanced.py`) and perform the following tests.

### Test 1: I2C Scan
Verify the STM32 can find the ESP32 on the bus.

```bash
# Initialize I2C1 at 100kHz
mimic> I2C_INIT 1 100000

# Scan bus
mimic> I2C_SCAN 1
```

**Expected Output:**
```
Scanning I2C1 bus...
Found device at 0x55
Scan complete. Found 1 devices.
```

### Test 2: Write Data
Write data to a specific register on the ESP32 (simulated).

```bash
# Write 0xAA 0xBB to register 0x10
mimic> I2C_WRITE 1 0x55 10 AA BB
```

**Expected ESP32 Serial Output:**
```
[RX] Received 3 bytes: [10] AA BB 
```

### Test 3: Read Data (Simulated Registers)
Read back the data you just wrote, or read default values.
The ESP32 sketch initializes registers with their index (Reg 0x05 = 0x05).

```bash
# Pattern: Write register address (0x10), then Read 1 byte
mimic> I2C_WRITE_READ 1 0x55 10 1
```

**Expected Master Output:**
```
OK: Read 1 bytes from 0x55: AA
```
*(Since we wrote 0xAA to register 0x10 in Test 2)*

### Test 4: Default Register Values
Read register 0x05 (should contain 0x05).

```bash
mimic> I2C_WRITE_READ 1 0x55 05 1
```

**Expected Output:**
```
OK: Read 1 bytes from 0x55: 05
```

### Test 5: Sequential Read
Read multiple bytes starting from 0x00.
Reg 0x00 is 0xBE, 0x01 is 0xEF.

```bash
mimic> I2C_WRITE_READ 1 0x55 00 2
```

**Expected Output:**
```
OK: Read 2 bytes from 0x55: BE EF
```

---

## Troubleshooting

**Problem: No device found**
- Check GND connection.
- Verify STM32 pins PB6/PB7 are used.
- Try `I2C_INIT 1 100000` again.
- Swap SDA/SCL lines (just in case).

**Problem: Data corruption**
- Ensure wires are short.
- Add 4.7kΩ pull-up resistors to 3.3V on SDA and SCLlines.
- Lower the speed is already at 100kHz.

**Problem: ESP32 not responding**
- Reset the ESP32 (press EN button).
- Check Serial Monitor for "I2C Init Failed" errors.
