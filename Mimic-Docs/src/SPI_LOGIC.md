# SPI Logic & Protocol

## Overview

SPI is a high-speed synchronous serial protocol. Mimic emulates SPI slave devices by monitoring the Chip Select (CS) and Clock (SCK) lines and responding with data on the MISO line.

## Supported Modes

Mimic supports all four standard SPI modes:
- **Mode 0:** CPOL=0, CPHA=0
- **Mode 1:** CPOL=0, CPHA=1
- **Mode 2:** CPOL=1, CPHA=0
- **Mode 3:** CPOL=1, CPHA=1

## Transaction Handling

When the CS line is pulled low, the firmware prepares for a transaction. It uses hardware interrupts to ensure that data is shifted out on the MISO line in perfect sync with the master's clock signal.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
