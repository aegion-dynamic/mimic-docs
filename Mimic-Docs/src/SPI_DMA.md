# SPI DMA Integration

## Overview

To handle very high bus speeds (up to 20 MHz) without burdening the CPU, Mimic utilizes the STM32's Direct Memory Access (DMA) controller for SPI transactions.

## How it Works

1. **Setup:** The CPU configures the DMA to point to the sensor's register shadow in SRAM.
2. **Transfer:** When the master starts a transaction, the DMA automatically moves data between the SPI peripheral and SRAM.
3. **Completion:** An interrupt is triggered only after the entire block of data has been transferred, allowing the CPU to focus on parsing other commands.

## Benefits

Using DMA reduces the latency between bytes and ensures that Mimic can keep up with fast masters even when emulating complex sensors with large data buffers.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
