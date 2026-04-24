# 🛠 Sensors API: Writing Your Own Mock

One of the most powerful features of Mimic is the ability to write your own sensor simulation classes. This allows you to "mock" any I2C or SPI device.

## 🧱 The Simulator Structure

New sensors should be placed in `mimic/sensors/` and should follow this pattern:

```python
import logging
from mimic.bridge import I2CSlave, MimicBridge

class MyNewSensor:
    def __init__(self, bridge: MimicBridge, address: int):
        self.i2c = I2CSlave(bridge)
        self.address = address
        # 1. Define your internal registers
        self.registers = {
            0x01: [0x55], # Status Reg
            0x02: [0x00, 0x00] # Data Reg
        }

    def start(self):
        # 2. Init the hardware
        self.i2c.init(self.address)
        
        while True:
            # 3. Wait for the Master to talk
            received = self.i2c.wait_for_write(1)
            
            if received:
                reg = received[0]
                
                # Handle Writes
                if len(received) > 1:
                    self.registers[reg] = received[1:]
                    continue
                
                # Handle Reads
                data = self.registers.get(reg, [0xFF])
                self.i2c.respond(data)
```

## 📋 Best Practices

1.  **Block Sizes**: Keep your internal register lists as byte arrays.
2.  **Clock Stretching**: The `wait_for_write` method is blocking. This is good because it triggers I2C clock stretching on the STM32, ensuring the master waits for your Python code to process the logic.
3.  **Real-time Data**: You can update your internal registers from a separate thread or based on another input, and the Master will get the "live" value on the next I2C read.

## 🚀 Adding to CLI
To make your new sensor available in the `mimic-sim` command, add it to the choice list in `mimic/cli.py`.
