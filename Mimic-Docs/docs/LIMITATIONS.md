# Mimic System Limitations

Mimic uses an STM32F401 (Black Pill) coupled with a Python-based Host Bridge. While powerful, it has specific physical and timing constraints.

### 1. I2C (Inter-Integrated Circuit)
*   **Max Speed (Standard)**: 100 kHz.
*   **Max Speed (Fast)**: 400 kHz (Requires physical 4.7kΩ pull-up resistors).
*   **No-Resistor Operation**: Stable at 100 kHz due to firmware-level interrupt optimization, but requires short jumper wires (< 15cm).
*   **Slave Address**: Supports any 7-bit address (configured via simulation script).

### 2. SPI (Serial Peripheral Interface)
*   **Mode**: Slave Only.
*   **Max Clock**: 18 MHz (Tested). 
*   **Constraints**: Requires a dedicated Chip Select (CS) pin. Extremely sensitive to wire length at speeds above 1MHz.

### 3. Host-to-Mimic Bridge (Serial)
*   **Standard Baudrate**: 115200 bps.
*   **Update Latency**: ~1.5ms per register update.
*   **Buffer Depth**: 256-byte Shadow Register Map. The STM32 serves data instantly from RAM, but updates from the Laptop are subject to OS serial scheduling.

### 4. Hardware Constraints
*   **Logic Level**: 3.3V Only. Connecting to 5V devices (like older Arduino Unos) requires a logic level shifter.
*   **Internal Pull-ups**: ~40kΩ. Significant for I2C; not recommended for high-speed operation without external support.
