# Real-Time Diagnostics

## Overview

Mimic includes a diagnostic layer that monitors the health and performance of the system in real-time. This helps in identifying communication issues, protocol errors, or hardware failures during development.

## Monitored Metrics

- **Bus Load:** The percentage of available bandwidth being used on the I2C or SPI bus.
- **Packet Loss:** The number of commands sent by the host that failed to receive a valid acknowledgement.
- **Timing Jitter:** Variations in the execution time of periodic tasks (e.g., sensor data updates).
- **MCU Temperature:** The internal temperature of the STM32 chip.

## Diagnostic Commands

You can query the diagnostic state using the CLI:
```bash
# Get a summary of the system health
mimic diagnostics status

# Monitor bus activity in real-time
mimic diagnostics monitor --bus i2c1
```

## Logging

Diagnostic data can be exported to a CSV file or streamed to a dashboard for long-term analysis of hardware stability.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
