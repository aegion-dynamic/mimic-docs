# System Architecture

The Mimic framework bridges a host computer to physical hardware peripherals. The core architecture relies on an STM32-based hardware component running the Mimic Firmware, connected to a Python-based host bridge over a serial protocol.

## High-Level Architecture

The following block diagram illustrates the flow of data from the host machine to the hardware peripherals:

```mermaid
%%{init: {
  'theme': 'base',
  'themeVariables': {
    'primaryColor': '#1a1a2e',
    'primaryTextColor': '#ffffff',
    'primaryBorderColor': '#3a3a5c',
    'lineColor': '#5a5a7a',
    'secondaryColor': '#16213e',
    'tertiaryColor': '#0f3460',
    'fontFamily': 'Space Mono, monospace',
    'fontSize': '13px',
    'noteBkgColor': '#1a1a2e',
    'noteTextColor': '#ffffff'
  },
  'flowchart': {
    'padding': 24,
    'nodeSpacing': 50,
    'rankSpacing': 50,
    'curve': 'basis'
  }
}}%%
graph TD
    classDef host fill:#1b3a4b,stroke:#4a90a4,stroke-width:1.5px,color:#e0e0e0
    classDef core fill:#4a2040,stroke:#9b4dca,stroke-width:2px,color:#ffffff
    classDef iface fill:#2d4a22,stroke:#6dae4f,stroke-width:1.5px,color:#e0e0e0
    classDef sensor fill:#4a3520,stroke:#d4915a,stroke-width:1.5px,color:#e0e0e0

    CLI["Mimic CLI"]:::host
    PyLib["MimicBridge Library"]:::host
    Sensors["MimicSensors Modules"]:::host

    Core["Mimic Firmware Core"]:::core

    I2C["I2C Emulation"]:::iface
    SPI["SPI Emulation"]:::iface
    UART["UART Interface"]:::iface
    GPIO["GPIO Control"]:::iface

    MPU["MPU6050"]:::sensor
    BMP["BMP280"]:::sensor
    GPS["GPS NMEA"]:::sensor
    LED["LED / GPIO"]:::sensor

    CLI --> PyLib
    Sensors --> PyLib
    PyLib -->|USB CDC| Core
    Core --> I2C
    Core --> SPI
    Core --> UART
    Core --> GPIO
    I2C --> MPU
    SPI --> BMP
    UART --> GPS
    GPIO --> LED
```

## How It Works

1. **Python Bridge:** The user sends a command (e.g., `PIN_HIGH PC13` or `simulate mpu6050`) using the Python library (`mimic-fw`).
2. **Serial Transmission:** The `MimicBridge` translates this into a compact string protocol and transmits it over USB.
3. **Firmware Execution:** The STM32F411 micro-controller interprets the packet. It bypasses heavy abstractions (like standard HALs where necessary) to maintain high-precision, low-latency emulation logic.
4. **Peripheral Activity:** Signal levels are changed on GPIOs, or dummy register shadows are updated to fool the Master device into thinking a real MPU6050 or BMP280 is connected.

## Command Execution Flow

```mermaid
%%{init: {
  'theme': 'base',
  'themeVariables': {
    'primaryColor': '#1b3a4b',
    'primaryTextColor': '#ffffff',
    'primaryBorderColor': '#4a90a4',
    'lineColor': '#5a5a7a',
    'secondaryColor': '#4a2040',
    'tertiaryColor': '#2d4a22',
    'fontFamily': 'Space Mono, monospace',
    'fontSize': '13px'
  }
}}%%
sequenceDiagram
    box #1b3a4b Host Machine (Python)
    participant User as User / Script
    participant Bridge as Python Bridge
    end
    box #4a2040 STM32 Core
    participant MCU as STM32 Firmware
    end
    box #2d4a22 Peripherals
    participant HW as Hardware Pins
    end

    User->>Bridge: bridge.pin_high('PC13')
    Bridge->>MCU: "PIN_HIGH PC13\n"
    Note over MCU: Parse Command
    MCU->>HW: Set PC13 to HIGH
    HW-->>MCU: Pin State Updated
    MCU-->>Bridge: "OK\n"
    Bridge-->>User: True
```

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
