# UART Data Flow

## Overview

The UART interface is used for both host communication (USB CDC) and peripheral emulation (e.g., GPS sentences). Mimic manages the flow of data to ensure that serial buffers do not overflow.

## Host Communication

Data from the host can be received via:
1. **USB-C Port:** Using the USB CDC protocol.
2. **UART2 (TTL):** Using PA2 (TX) and PA3 (RX) for external TTL control.

The firmware uses a circular buffer to store incoming characters until they can be parsed by the command engine.

## Peripheral Emulation

When emulating a UART device like a GPS module:
- The firmware generates data packets (e.g., NMEA sentences).
- These packets are transmitted via **UART1** using the physical TX pin (**PA9**).
- A background process ensures that data is streamed at the correct baud rate (typically 9600 for GPS).

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
