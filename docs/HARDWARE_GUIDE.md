# 🔌 BlackPill Hardware Guide

This guide describes the physical interface of the Mimic board and safety precautions.

## 📍 Pin Topology (F411CEU6)

### Power Pins
*   **3V3**: Output from the onboard regulator or Input if powering externally. **Max 3.6V.**
*   **5V**: Power input (from USB or external). **Do not connect to STM32 I/O pins.**
*   **GND**: Common ground reference.

---

### Critical Peripheral Pins

| Pin | Function | 5V Tolerant? | Notes |
| :--- | :--- | :--- | :--- |
| **PA9** | UART1 TX | **YES** | Host Interface (Connect to Adapter RX) |
| **PA10**| UART1 RX | **YES** | Host Interface (Connect to Adapter TX) |
| **PB8** | I2C1 SCL | **YES** | Primary I2C Clock |
| **PB9** | I2C1 SDA | **YES** | Primary I2C Data |
| **PC13**| Onboard LED | **NO** | Active LOW (Low = ON) |

---

## ⚡ Electrical Safety

### 1. 5V Tolerance
Many pins on the STM32F411 are **5V tolerant** (marked `FT` in the datasheet). However, PA4, PA5, and PB5 are NOT 5V tolerant on some packages.
*   **Safest Practice**: Always use 3.3V logic or level shifters when talking to 5V systems.

### 2. Output Current
Each GPIO can source/sink approximately **20mA**.
*   **Warning**: Do not drive motors, solenoids, or high-power LEDs directly from the pins. Always use a MOSFET or transistor.

### 3. ADC Range
If you implement ADC commands, remember the range is **0V to 3.3V**. Voltages above 3.3V will permanently damage the ADC circuitry.

---

## 💡 Troubleshooting
*   **Board not detected**: Ensure the Blue LED is on. This indicates the 3.3V regulator is working.
*   **No Serial Response**: Swap TX and RX wires. Check that your baud rate is exactly `115200`.
*   **Stuck in Loop**: Press the **NRST** (Reset) button on the board to restart the firmware.
