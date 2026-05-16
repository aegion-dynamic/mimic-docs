# Python Bridge: Serial

## Overview

The Serial Bridge is the primary communication link between the host computer and the Mimic hardware. It uses the USB CDC (Communication Device Class) interface to transmit commands and receive sensor data.

## Communication Protocol

Mimic uses a text-based protocol for simplicity and ease of debugging. Each command is sent as a string terminated by a newline character.

- **Request:** `PIN_HIGH PC13\n`
- **Response:** `OK\n`

## Serial Configuration

The bridge automatically configures the serial port with the following settings:
- **Baud Rate:** 115200 (default)
- **Data Bits:** 8
- **Parity:** None
- **Stop Bits:** 1

## Usage in Python

The `MimicBridge` class abstracts the serial communication. You can connect via the USB-C port (typically `ttyACM0`) or a TTL adapter on UART2 (typically `ttyUSB0`):

```python
from mimic import MimicBridge

# Connect via onboard USB CDC
bridge = MimicBridge(port='/dev/ttyACM0')

# OR Connect via UART2 (TTL Adapter)
# bridge = MimicBridge(port='/dev/ttyUSB0')

bridge.pin_high('PC13')
```

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
