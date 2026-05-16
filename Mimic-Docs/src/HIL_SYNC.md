# HIL Synchronization

## Overview

Hardware-in-the-Loop (HIL) synchronization ensures that the simulation time on the host computer is aligned with the physical clock on the Mimic hardware. This is crucial for accurate sensor emulation and control system testing.

## Mechanism

Mimic uses a heartbeat-based synchronization protocol:
1. The hardware MCU generates a stable clock signal or heartbeat packet.
2. The Python bridge monitors this heartbeat to adjust its internal timing.
3. If the host falls behind (e.g., due to system load), it can request a state synchronization from the MCU to catch up.

## Key Features

- **Drift Correction:** Automatically compensates for the small differences between the host and MCU oscillators.
- **Timestamping:** Every packet sent from the hardware includes a high-resolution timestamp (in microseconds) relative to the system boot.
- **Latency Monitoring:** The bridge measures the round-trip time of commands to provide an estimate of the communication latency.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
