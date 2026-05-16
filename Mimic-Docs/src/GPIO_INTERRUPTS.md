# GPIO Interrupts

## Overview

Mimic can monitor state changes on any digital input pin using hardware interrupts. This is used for capturing external events such as button presses, sensor signals, or protocol handshake lines.

## How it Works

1. **Configuration:** The user specifies which pin to monitor and the trigger condition (Rising, Falling, or Both edges).
2. **Detection:** When the condition is met, the STM32's EXTI (External Interrupt) controller triggers a handler.
3. **Reporting:** The firmware logs the event and sends a notification packet to the host bridge.

## Performance

The use of hardware interrupts ensures that even very brief pulses (in the microsecond range) are captured reliably without polling the pin state in the main loop.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
