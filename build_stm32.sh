#!/bin/bash

# Build script for STM32F411 UART Firmware
# Compiles and creates ELF, BIN, and HEX files

echo "Building STM32F411 UART Firmware..."

# Check if ARM toolchain is installed
if ! command -v arm-none-eabi-gcc &> /dev/null; then
    echo "ERROR: arm-none-eabi-gcc not found. Install ARM toolchain:"
    echo "  Ubuntu/Debian: sudo apt-get install arm-none-eabi-gcc"
    echo "  macOS: brew install arm-none-eabi-gcc"
    exit 1
fi

# Compiler flags
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
CFLAGS="-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard"
CFLAGS="$CFLAGS -fno-common -O2 -g"
LDFLAGS="-mcpu=cortex-m4 -mthumb -nostartfiles"

# Output files
OUTPUT_ELF="stm32_uart_firmware.elf"
OUTPUT_BIN="stm32_uart_firmware.bin"
OUTPUT_HEX="stm32_uart_firmware.hex"

# Compile
echo "Compiling..."
$CC $CFLAGS -c stm32_uart_firmware.c -o stm32_uart_firmware.o

if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed"
    exit 1
fi

# Link
echo "Linking..."
$CC $LDFLAGS stm32_uart_firmware.o -o $OUTPUT_ELF

if [ $? -ne 0 ]; then
    echo "ERROR: Linking failed"
    exit 1
fi

# Generate binary
echo "Generating binary..."
$OBJCOPY -O binary $OUTPUT_ELF $OUTPUT_BIN
$OBJCOPY -O ihex $OUTPUT_ELF $OUTPUT_HEX

# Show info
echo ""
echo "Build complete!"
echo "Files:"
echo "  $OUTPUT_ELF   - Executable"
echo "  $OUTPUT_BIN   - Binary (for STM32)"
echo "  $OUTPUT_HEX   - Intel HEX (for ST-LINK/OpenOCD)"
echo ""
echo "To flash with OpenOCD:"
echo "  openocd -f interface/stlink.cfg -f target/stm32f4x.cfg"
echo "  # In another terminal:"
echo "  arm-none-eabi-gdb $OUTPUT_ELF"
echo "  > target extended-remote :3333"
echo "  > file $OUTPUT_ELF"
echo "  > load"
echo "  > cont"
echo ""
echo "To flash with st-flash:"
echo "  st-flash write $OUTPUT_BIN 0x8000000"

# Clean up
rm -f stm32_uart_firmware.o
