# Python Bridge: Async

## Overview

For applications requiring high-frequency data logging or simultaneous control of multiple peripherals, Mimic provides an asynchronous API based on Python's `asyncio` library.

## Why Use Async?

Traditional serial communication is synchronous, meaning the program waits for a response after every command. This can limit performance during complex simulations. The Async Bridge allows you to:
- Send multiple commands without waiting for immediate responses.
- Continuously stream sensor data in the background.
- Build responsive user interfaces that don't freeze during hardware I/O.

## Usage Example

```python
import asyncio
from mimic import AsyncMimicBridge

async def main():
    bridge = AsyncMimicBridge(port='/dev/ttyUSB0')
    await bridge.connect()
    
    # Task 1: Blink an LED
    asyncio.create_task(bridge.blink('PC13', interval=0.5))
    
    # Task 2: Read sensor data
    while True:
        data = await bridge.read_sensor('mpu6050')
        print(f"Accel: {data}")
        await asyncio.sleep(0.1)

asyncio.run(main())
```

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
