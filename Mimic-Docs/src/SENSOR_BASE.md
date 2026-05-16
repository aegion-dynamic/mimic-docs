# Abstract Sensor Base

## Overview

The `AbstractSensorBase` is a Python class that serves as the foundation for all sensor emulation modules in Mimic. It provides the core logic for managing register maps and handling protocol-specific transactions.

## Core Responsibilities

- **Register Shadowing:** Maintains a virtual copy of the sensor's internal registers.
- **Protocol Mapping:** Defines how register reads/writes translate to physical I2C or SPI transactions.
- **Data Generation Hooks:** Provides methods for updating register values based on simulation logic (e.g., generating random noise for an accelerometer).

## Key Methods

- `read_register(address)`: Returns the value currently stored in a virtual register.
- `write_register(address, value)`: Updates a virtual register and triggers any associated side effects.
- `update()`: A periodic hook used to calculate new sensor data (e.g., incrementing a counter or reading from a data file).

## Example Implementation

To create a new sensor, you simply need to inherit from this class and define your register map:

```python
class MyCustomSensor(AbstractSensorBase):
    def __init__(self):
        super().__init__()
        self.registers = {
            0x00: 0x45, # Device ID
            0x01: 0x00  # Data Register
        }
```

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
