# Environment Setup & Getting Started

This guide provides the technical requirements and procedures for setting up the Mimic environment, compiling the firmware, and establishing communication with the STM32 hardware.

**Maintainer:** Karthik Sarvan


## 1. Required Hardware

To deploy and utilize the Mimic framework, the following hardware is required:
*   **MCU Development Board:** STM32F411CEU6 (**Black Pill**).
*   **Programmer:** ST-Link V2 (USB dongle).
*   **Peripherals (Optional):** Jumper wires and standard sensors for emulation testing (e.g., MPU6050, BMP280, GPS).

## 2. Cloning the Source

Clone the primary firmware repository from the Aegion Dynamic organization:

```bash
git clone https://github.com/aegion-dynamic/Mimic-Firmware.git
cd Mimic-Firmware/STM32
```

## 3. Compiling the Firmware

Compilation requires the `arm-none-eabi-gcc` toolchain and the `make` utility.

Execute the build process:
```bash
make
```
The build system will generate `mimic.bin`, `mimic.hex`, and `mimic.elf` binaries within the `build/` directory.

## 4. Hardware Injection

1. Interface the **ST-Link V2** with the Black Pill:
   *   `3.3V` -> `3.3V`
   *   `GND` -> `GND`
   *   `SWDIO` -> `DIO`
   *   `SWCLK` -> `CLK`
2. Connect the ST-Link to your host machine.
3. Deploy the binary using the `st-flash` utility:

```bash
st-flash write build/mimic.bin 0x8000000
```
*Note: After successful deployment, disconnect the ST-Link and connect the Black Pill directly via USB-C to begin host communication.*

## 5. Verification: Onboard Diagnostics

Once flashed, verify the bridge communication using the Python client. The onboard LED (mapped to `PC13`) serves as the default diagnostic indicator.

Ensure the Python environment is configured:
```bash
pip install mimic-fw
```

Initialize the interactive hardware shell:
```bash
mimic
```

Execute the following commands to test GPIO control:
```text
> PIN_HIGH PC13
OK
> PIN_LOW PC13
OK
```

If the onboard LED responds to these commands, the Mimic Hardware Bridge is correctly initialized and ready for sensor emulation.

