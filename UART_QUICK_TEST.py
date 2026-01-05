#!/usr/bin/env python3
"""
Quick UART Test Script - Copy-paste ready examples
Run individual examples to test UART loopback
"""

# BEFORE RUNNING: Connect PA9 to PA10 with a jumper wire
# Then run: python3 Mimic.py and copy-paste the commands below

"""
=== BASIC LOOPBACK TEST ===

1. Show configuration
stm32> uart_config A 9 A 10 baud=9600

2. Send text
stm32> uart_send A 9 "Hello" baud=9600

3. Receive text (should get "Hello" back)
stm32> uart_recv A 10 timeout=2000 baud=9600


=== HEX FORMAT TEST ===

1. Send hex data
stm32> uart_send A 9 "0x41 0x42 0x43" format=hex baud=9600

2. Receive hex data
stm32> uart_recv A 10 format=hex timeout=2000 baud=9600
Expected: 0x41 0x42 0x43 (which is "ABC" in ASCII)


=== DIFFERENT BAUD RATES ===

# 19200 baud
stm32> uart_send A 9 "Test19200" baud=19200
stm32> uart_recv A 10 baud=19200 timeout=2000

# 38400 baud
stm32> uart_send A 9 "Test38400" baud=38400
stm32> uart_recv A 10 baud=38400 timeout=2000

# 115200 baud (high speed)
stm32> uart_send A 9 "Test115200" baud=115200
stm32> uart_recv A 10 baud=115200 timeout=2000


=== USING DIFFERENT PIN PAIRS ===

# Pins PD5 and PD6 (connect physically: PD5 <-> PD6)
stm32> uart_send D 5 "PortD" baud=9600
stm32> uart_recv D 6 baud=9600 timeout=2000

# Pins PB10 and PB11 (connect: PB10 <-> PB11)
stm32> uart_send B 10 "PortB" baud=9600
stm32> uart_recv B 11 baud=9600 timeout=2000

# Pins PE0 and PE1 (connect: PE0 <-> PE1)
stm32> uart_send E 0 "PortE" baud=9600
stm32> uart_recv E 1 baud=9600 timeout=2000


=== MONITORING MODE ===

# Monitor for 10 seconds
stm32> uart_monitor A 10 duration=10000 interval=100 baud=9600

# Then send data in another shell window:
stm32> uart_send A 9 "Message1" baud=9600
stm32> uart_send A 9 "Message2" baud=9600

# Monitor window shows timestamps and data:
# [1000ms] RX: Message1
# [2100ms] RX: Message2


=== COMBINED PARAMETERS ===

# Everything custom
stm32> uart_config A 9 A 10 baud=57600

stm32> uart_send A 9 "0x12 0x34 0x56" format=hex baud=57600

stm32> uart_recv A 10 format=hex baud=57600 timeout=3000


=== STRESS TEST ===

# Send long message
stm32> uart_send A 9 "The quick brown fox jumps over the lazy dog" baud=9600

# Receive it back
stm32> uart_recv A 10 timeout=3000 baud=9600

# Send multiple hex bytes
stm32> uart_send A 9 "0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F" format=hex baud=9600

# Receive hex
stm32> uart_recv A 10 format=hex timeout=2000 baud=9600


=== PROTOCOL SIMULATION ===

Imagine sensor sends: "SENSOR_ID:001|TEMP:25.5|HUM:60"

# Send simulated sensor data
stm32> uart_send A 9 "SENSOR_ID:001|TEMP:25.5|HUM:60" baud=9600

# Receive it
stm32> uart_recv A 10 timeout=2000 baud=9600


=== BINARY PROTOCOL TEST ===

Imagine protocol: [START:0xAA] [CMD:0x01] [DATA1:0x12] [DATA2:0x34] [STOP:0x55]

# Send protocol frame
stm32> uart_send A 9 "0xAA 0x01 0x12 0x34 0x55" format=hex baud=9600

# Receive it
stm32> uart_recv A 10 format=hex timeout=2000 baud=9600


=== CONFIGURATION CHECK ===

# Before each test, verify pins
stm32> gpio_info A 9
stm32> gpio_info A 10

# Check UART settings
stm32> uart_config A 9 A 10 baud=9600


=== EDGE CASES ===

# Empty string (will timeout)
stm32> uart_recv A 10 timeout=500 baud=9600

# Very short timeout
stm32> uart_send A 9 "Fast" baud=9600
stm32> uart_recv A 10 timeout=100 baud=9600

# Single byte
stm32> uart_send A 9 "X" baud=9600
stm32> uart_recv A 10 timeout=1000 baud=9600

# Single hex byte
stm32> uart_send A 9 "0xFF" format=hex baud=9600
stm32> uart_recv A 10 format=hex timeout=1000 baud=9600


=== HELP COMMANDS ===

stm32> help
# Shows all available commands

stm32> gpio_out A 9
stm32> gpio_in A 10
stm32> gpio_read A 9
stm32> gpio_read A 10
# Test GPIO pins manually
"""

# Quick test function (if run as script)
def quick_test():
    """Quick validation of UART setup"""
    print(__doc__)
    print("\n" + "="*60)
    print("UART QUICK TEST GUIDE")
    print("="*60)
    print("\nRun Mimic.py and copy-paste the commands above.")
    print("\nPhysical Setup Required:")
    print("  Wire PA9 ---[jumper]--- PA10  (for PA9/PA10 tests)")
    print("  or use any other port pair")
    print("\nExpected Results:")
    print("  - uart_send sends data via TX pin")
    print("  - uart_recv receives data via RX pin")
    print("  - In loopback, what you send you should receive")
    print("  - All parameters are optional with sensible defaults")
    print("\n" + "="*60)

if __name__ == "__main__":
    quick_test()
