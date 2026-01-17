# I2C Test Guide

This guide helps you verify the I2C implementation on the STM32F411 Discovery board using common I2C devices or a loopback test.

---

## 1. Loopback Test (No Device Needed)

This test connects the I2C SDA and SCL lines together to verify basic signal integrity (though I2C protocol requires a second device to ACK, so this just tests that the bus doesn't hang).

**Wiring:**
- Connect PB6 (SCL) to PB7 (SDA) via a 4.7kΩ resistor (optional but good practice)
- Or simply pull both lines high with 4.7kΩ resistors

**Test:**
```bash
# Initialize I2C1
I2C_INIT 1 100000

# Scan bus (should fail gracefully or find "ghosts" if lines shorted)
I2C_SCAN 1
```

---

## 2. Testing with MPU6050 (Gyro/Accel)

**Wiring (I2C1):**
- MPU6050 VCC -> STM32 3.3V
- MPU6050 GND -> STM32 GND
- MPU6050 SCL -> STM32 PB6
- MPU6050 SDA -> STM32 PB7

**Test Commands:**
```bash
# Initialize
I2C_INIT 1 400000

# Scan for device (should find 0x68 or 0x69)
I2C_SCAN 1

# Read WHO_AM_I register (0x75)
# Expected result: 0x68
I2C_WRITE_READ 1 0x68 75 1

# Read Accelerometer Data (starts at 0x3B, 6 bytes)
I2C_WRITE_READ 1 0x68 3B 6
```

---

## 3. Testing with SSD1306 OLED (0.96")

**Wiring (I2C1):**
- OLED VCC -> STM32 3.3V
- OLED GND -> STM32 GND
- OLED SCL -> STM32 PB6
- OLED SDA -> STM32 PB7

**Test Commands:**
```bash
# Initialize
I2C_INIT 1 400000

# Scan (should find 0x3C or 0x3D)
I2C_SCAN 1

# Turn Display OFF (Command 0xAE)
I2C_WRITE 1 0x3C 00 AE

# Turn Display ON (Command 0xAF)
I2C_WRITE 1 0x3C 00 AF

# Invert Display (Command 0xA7)
I2C_WRITE 1 0x3C 00 A7

# Normal Display (Command 0xA6)
I2C_WRITE 1 0x3C 00 A6
```

---

## 4. Testing with Multiple Devices

Connect both MPU6050 and OLED to the same I2C1 bus.

**Test Commands:**
```bash
# Initialize
I2C_INIT 1 400000

# Scan (should find both 0x68 and 0x3C)
I2C_SCAN 1

# Talk to MPU6050
I2C_WRITE_READ 1 0x68 75 1

# Talk to OLED
I2C_WRITE 1 0x3C 00 A7
```

---

## Troubleshooting

**I2C_SCAN identifies no devices:**
1. Check Pull-up Resistors: I2C needs 4.7kΩ pull-ups on SDA and SCL. Many modules have them built-in, but bare chips don't.
2. Check Wiring: SCL->SCL, SDA->SDA (not crossed like UART!).
3. Check Power: Ensure device has 3.3V.

**I2C_SCAN hangs:**
1. Short circuit on bus?
2. SDA/SCL shorted to GND?

**Incorrect Data:**
1. Reduce speed to 100000 (100kHz).
2. Check loose connections.
