# STM32F411 SPI Pinout Reference

Complete pinout reference for all 5 SPI peripherals on STM32F411 Discovery board.

---

## Quick Reference Table

| SPI | SCK | MISO | MOSI | CS | Max Speed | Bus | Notes |
|-----|-----|------|------|----|-----------|-----|-------|
| **SPI1** | PA5 | PA6 | PA7 | PA4 | 50 MHz | APB2 | Arduino headers |
| **SPI2** | PB13 | PB14 | PB15 | PB12 | 25 MHz | APB1 | GPIO headers |
| **SPI3** | PB3 | PB4 | PB5 | PA15 | 25 MHz | APB1 | Mixed ports |
| **SPI4** | PE2 | PE5 | PE6 | PE4 | 50 MHz | APB2 | Port E |
| **SPI5** | PE12 | PE13 | PE14 | PE11 | 50 MHz | APB2 | Port E |

---

## Detailed Pinouts

### SPI1 (Recommended - Arduino Compatible)
**Bus**: APB2 (High Speed)  
**Max Speed**: 50 MHz  
**Accessibility**: ⭐⭐⭐⭐⭐ (Arduino headers)

| Function | Pin | Arduino | Alt Function |
|----------|-----|---------|--------------|
| SCK      | PA5 | D13     | AF5 |
| MISO     | PA6 | D12     | AF5 |
| MOSI     | PA7 | D11     | AF5 |
| NSS/CS   | PA4 | A2      | AF5 |

**Initialization:**
```bash
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4
```

**Use Cases:**
- SD cards
- SPI displays
- External flash memory
- General purpose SPI devices

---

### SPI2
**Bus**: APB1 (Standard Speed)  
**Max Speed**: 25 MHz  
**Accessibility**: ⭐⭐⭐ (GPIO headers)

| Function | Pin | Alt Function |
|----------|-----|--------------|
| SCK      | PB13 | AF5 |
| MISO     | PB14 | AF5 |
| MOSI     | PB15 | AF5 |
| NSS/CS   | PB12 | AF5 |

**Initialization:**
```bash
SPI_INIT 2 MASTER 1000000 0 0 8 MSB B12
```

**Use Cases:**
- Sensors (accelerometers, gyroscopes)
- ADC/DAC chips
- Radio modules (nRF24L01)

---

### SPI3
**Bus**: APB1 (Standard Speed)  
**Max Speed**: 25 MHz  
**Accessibility**: ⭐⭐⭐ (GPIO headers)

| Function | Pin | Alt Function |
|----------|-----|--------------|
| SCK      | PB3  | AF6 |
| MISO     | PB4  | AF6 |
| MOSI     | PB5  | AF6 |
| NSS/CS   | PA15 | AF6 |

**Initialization:**
```bash
SPI_INIT 3 MASTER 1000000 0 0 8 MSB A15
```

**Use Cases:**
- Multiple SPI device setups
- Backup SPI interface
- Sensor networks

---

### SPI4
**Bus**: APB2 (High Speed)  
**Max Speed**: 50 MHz  
**Accessibility**: ⭐⭐ (Port E - check board)

| Function | Pin | Alt Function |
|----------|-----|--------------|
| SCK      | PE2  | AF5 |
| MISO     | PE5  | AF5 |
| MOSI     | PE6  | AF5 |
| NSS/CS   | PE4  | AF5 |

**Initialization:**
```bash
SPI_INIT 4 MASTER 1000000 0 0 8 MSB E4
```

**Use Cases:**
- High-speed data acquisition
- Fast ADCs
- High-bandwidth sensors

---

### SPI5
**Bus**: APB2 (High Speed)  
**Max Speed**: 50 MHz  
**Accessibility**: ⭐⭐ (Port E - check board)

| Function | Pin | Alt Function |
|----------|-----|--------------|
| SCK      | PE12 | AF6 |
| MISO     | PE13 | AF6 |
| MOSI     | PE14 | AF6 |
| NSS/CS   | PE11 | AF6 |

**Initialization:**
```bash
SPI_INIT 5 MASTER 1000000 0 0 8 MSB E11
```

**Use Cases:**
- Dedicated high-speed interface
- Parallel SPI operations
- Multi-device systems

---

## Common SPI Devices & Settings

### SD Card
```bash
# SD cards use SPI Mode 0, slow speed initially
SPI_INIT 1 MASTER 400000 0 0 8 MSB A4
SPI_TRANSFER 1 FF FF FF FF  # Dummy clocks
# After init, can increase to 25MHz
```

### nRF24L01 Radio
```bash
# Mode 0, up to 10MHz
SPI_INIT 2 MASTER 8000000 0 0 8 MSB B12
SPI_TRANSFER 2 07  # Read STATUS register
```

### MPU6050 (SPI version)
```bash
# Mode 0 or 3, up to 1MHz
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4
SPI_TRANSFER 1 75 00  # Read WHO_AM_I
```

### MAX7219 LED Driver
```bash
# Mode 0, up to 10MHz
SPI_INIT 1 MASTER 10000000 0 0 8 MSB A4
SPI_TRANSFER 1 0C 01  # Normal operation
```

### W25Q Flash Memory
```bash
# Mode 0 or 3, up to 104MHz (use 50MHz max on STM32)
SPI_INIT 1 MASTER 50000000 0 0 8 MSB A4
SPI_TRANSFER 1 9F  # Read JEDEC ID
```

### ADS1220 ADC
```bash
# Mode 1, up to 4MHz
SPI_INIT 1 MASTER 4000000 0 1 8 MSB A4
SPI_TRANSFER 1 20  # Read register
```

---

## SPI Mode Reference

| Mode | CPOL | CPHA | Clock Idle | Sample Edge | Common Devices |
|------|------|------|------------|-------------|----------------|
| 0    | 0    | 0    | Low        | Rising      | SD Card, nRF24L01, MAX7219 |
| 1    | 0    | 1    | Low        | Falling     | ADS1220, Some ADCs |
| 2    | 1    | 0    | High       | Falling     | Rare |
| 3    | 1    | 1    | High       | Rising      | W25Q Flash, MPU6050 |

**Command Examples:**
```bash
# Mode 0 (most common)
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4

# Mode 1
SPI_INIT 1 MASTER 1000000 0 1 8 MSB A4

# Mode 2
SPI_INIT 1 MASTER 1000000 1 0 8 MSB A4

# Mode 3
SPI_INIT 1 MASTER 1000000 1 1 8 MSB A4
```

---

## Testing Guide

### Test 1: Loopback Test (No External Device)
```bash
# Connect MOSI to MISO (PA7 to PA6 for SPI1)
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4
SPI_TRANSFER 1 DE AD BE EF
# Expected: RX: DE AD BE EF
```

### Test 2: ESP32 Echo Test
```bash
# Wire ESP32 to SPI1
# ESP32: Set to echo mode (e)
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4
SPI_TRANSFER 1 AA BB CC DD
# Expected: RX: AA BB CC DD
```

### Test 3: All SPIs Sequential Test
```bash
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4
SPI_TRANSFER 1 11 11 11 11

SPI_INIT 2 MASTER 1000000 0 0 8 MSB B12
SPI_TRANSFER 2 22 22 22 22

SPI_INIT 3 MASTER 1000000 0 0 8 MSB A15
SPI_TRANSFER 3 33 33 33 33

SPI_INIT 4 MASTER 1000000 0 0 8 MSB E4
SPI_TRANSFER 4 44 44 44 44

SPI_INIT 5 MASTER 1000000 0 0 8 MSB E11
SPI_TRANSFER 5 55 55 55 55

SPI_STATUS  # Check all initialized
```

### Test 4: Different Speeds
```bash
SPI_INIT 1 MASTER 100000 0 0 8 MSB A4    # 100kHz
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4   # 1MHz
SPI_INIT 1 MASTER 10000000 0 0 8 MSB A4  # 10MHz
SPI_INIT 1 MASTER 50000000 0 0 8 MSB A4  # 50MHz (max)
```

### Test 5: 16-bit Mode
```bash
SPI_INIT 1 MASTER 1000000 0 0 16 MSB A4
SPI_TRANSFER 1 ABCD 1234
```

---

## Wiring Examples

### Single Device (e.g., SD Card on SPI1)
```
STM32          SD Card
PA5 (SCK)  →   CLK
PA6 (MISO) ←   DO
PA7 (MOSI) →   DI
PA4 (CS)   →   CS
3.3V       →   VCC
GND        →   GND
```

### Multiple Devices (Shared Bus)
```
STM32          Device 1    Device 2
PA5 (SCK)  →   CLK     →   CLK
PA6 (MISO) ←   MISO    ←   MISO
PA7 (MOSI) →   MOSI    →   MOSI
PA4 (CS1)  →   CS
PB0 (CS2)  →              CS
```

**Commands:**
```bash
# Initialize with first CS
SPI_INIT 1 MASTER 1000000 0 0 8 MSB A4

# Talk to Device 1
SPI_TRANSFER 1 AA BB CC DD

# Switch to Device 2 (manual CS)
SPI_CS A4 HIGH  # Deselect device 1
SPI_CS B0 LOW   # Select device 2
SPI_TRANSFER 1 11 22 33 44
SPI_CS B0 HIGH  # Deselect device 2
```

---

## Troubleshooting

### No Response from Device
1. **Check wiring** - Verify all connections
2. **Check voltage** - Ensure 3.3V (not 5V!)
3. **Check SPI mode** - Try different CPOL/CPHA
4. **Lower speed** - Start with 100kHz
5. **Check CS polarity** - Some devices use active HIGH

### Garbled Data
1. **Reduce speed** - Try 100kHz
2. **Check mode** - Verify CPOL/CPHA match device
3. **Check wiring** - Ensure short wires (<20cm)
4. **Add ground** - Use twisted pair for SCK/MOSI

### Intermittent Errors
1. **Add delays** - Some devices need setup time
2. **Check power** - Ensure stable 3.3V
3. **Reduce speed** - Lower clock frequency
4. **Check capacitors** - Add 0.1µF near device

---

## Speed Selection Guide

| Application | Recommended Speed | SPI |
|-------------|-------------------|-----|
| SD Card Init | 400 kHz | Any |
| SD Card Data | 25 MHz | SPI1/4/5 |
| Sensors | 1-10 MHz | Any |
| Flash Memory | 25-50 MHz | SPI1/4/5 |
| Displays | 10-40 MHz | SPI1/4/5 |
| Radio Modules | 8-10 MHz | Any |
| ADC/DAC | 1-20 MHz | Any |

---

## Best Practices

### ✅ DO
- Start with low speed (100kHz-1MHz) for testing
- Use automatic CS control when possible
- Check device datasheet for SPI mode
- Keep wires short (<20cm)
- Use common ground between devices
- Add 0.1µF capacitor near each device

### ❌ DON'T
- Don't exceed device max speed
- Don't use 5V devices without level shifters
- Don't forget pull-ups on MISO (if needed)
- Don't share CS between different devices
- Don't use long wires at high speeds
- Don't forget to initialize before transfer

---

## Quick Command Reference

```bash
# Initialize SPI with auto CS
SPI_INIT <1-5> MASTER <SPEED> <CPOL> <CPHA> <SIZE> <ORDER> <CS_PIN>

# Transfer data (auto CS)
SPI_TRANSFER <1-5> <HEX_DATA>

# Send only
SPI_SEND <1-5> <HEX_DATA>

# Receive only
SPI_RECV <1-5> <LENGTH>

# Manual CS control
SPI_CS <PIN> <HIGH|LOW>

# Check status
SPI_STATUS
```

---

## Next Steps

After verifying SPI works:
1. Test with actual SPI device
2. Try different SPI modes
3. Test at various speeds
4. Implement device-specific protocols
5. Move to I2C implementation

---

**For I2C implementation, see: I2C_PINOUT_REFERENCE.md**
