# Interrupt Processing

## Overview

The Mimic firmware uses interrupt-driven processing to handle time-critical peripheral events. This ensures that protocol timing for I2C and SPI remains consistent even when the main loop is busy parsing commands from the host.

## Implementation

- **External Interrupts (EXTI):** Used for monitoring GPIO state changes and triggering protocol responses.
- **Peripheral Interrupts:** Dedicated handlers for I2C and SPI bus events (Start, Stop, Address Match, Data Transmit/Receive).
- **Prioritization:** Critical bus timing interrupts are assigned higher priority to prevent data loss or bus timeouts.

## Execution Flow

1. A hardware event (e.g., I2C Start condition) triggers a peripheral interrupt.
2. The CPU pauses the main loop and executes the associated Interrupt Service Routine (ISR).
3. The ISR handles the immediate hardware requirement (e.g., loading a data register).
4. Control returns to the main loop to handle higher-level logic.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
