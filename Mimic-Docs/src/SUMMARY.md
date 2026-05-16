# Summary

- [Introduction](./README.md)
- [Wiring Guide](./WIRING.md)
- [Getting Started & Requirements](./SETUP.md)
- [System Architecture](./ARCH.md)

---
# MIMIC Firmware

- [Firmware Overview](./FW_OVERVIEW.md)
    - [GPIO Mapping](./GPIO_MAPPING.md)
    - [Interrupt Processing](./FW_INTERRUPTS.md)
    - [Memory Management](./FW_MEMORY.md)
    - [Build System](./BUILD_SYSTEM.md)
    - [Peripheral Control](./PERIPHERALS.md)
        - [I2C Timing & Registers](./I2C_TIMING.md)
        - [SPI Logic & DMA](./SPI_LOGIC.md)
        - [UART Data Flow](./UART_FLOW.md)

---
# MIMIC Bridge

- [Bridge Overview & CLI](./CLI_STRUCT.md)
    - [Python Bridge: Serial](./BRIDGE_SERIAL.md)
    - [Python Bridge: Async](./BRIDGE_ASYNC.md)
    - [HIL Synchronization](./HIL_SYNC.md)
    - [Real-Time Diagnostics](./DIAGNOSTICS.md)

---
# MIMIC Sensors & Contribution

- [Sensors Overview](./SENSORS_OVERVIEW.md)
    - [Abstract Sensor Base](./SENSOR_BASE.md)
    - [MPU6050 Motion Sensor](./MPU_MOTION.md)
        - [MPU6050 Registers](./MPU_REGISTERS.md)
    - [BMP280 Environment Sensor](./BMP_PRESSURE.md)
        - [BMP280 Calibration](./BMP_CALIBRATION.md)
    - [GPS Module](./GPS_NMEA.md)
        - [GPS Trajectory](./GPS_TRAJECTORY.md)

---
# Appendix

- [Troubleshooting](./TROUBLESHOOTING.md)
- [System Limitations](./LIMITATIONS.md)
