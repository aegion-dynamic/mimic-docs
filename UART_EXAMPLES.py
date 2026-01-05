"""
UART PROTOCOL TESTING - QUICK START GUIDE
STM32F411 Discovery Board via GPIO Bit-Banging
"""

# ============================================================================
# ANSWERING YOUR QUESTIONS:
# ============================================================================

# Q: Can I use any pins for communication?
# A: YES! Any GPIO pin on any port (A-E, pins 0-15) can be used.
#    You're not limited to dedicated UART pins.

# Q: Are there defaults for baud rate and data format?
# A: YES! Here are the defaults:
#    - baud: 9600 (9600, 19200, 38400, 57600, 115200, 230400 supported)
#    - format: ascii (or 'hex' for hex values)
#    - timeout: 1000ms
#    - duration: 10000ms (for monitor)
#    - interval: 100ms (for monitor)

# ============================================================================
# COMMAND EXAMPLES
# ============================================================================

# --- BASIC UART SEND/RECEIVE ---

# 1. Simple send with default baud (9600)
uart_send A 9 "Hello"

# 2. Send with custom baud rate
uart_send A 9 "Hello" baud=115200

# 3. Send hex data
uart_send A 9 "0xAA 0xBB 0xCC" format=hex

# 4. Send hex data at 19200 baud
uart_send A 9 "0x48 0x65" baud=19200 format=hex

# 5. Receive with default timeout (1000ms)
uart_recv A 10

# 6. Receive with longer timeout
uart_recv A 10 timeout=5000

# 7. Receive as hex format
uart_recv A 10 format=hex baud=115200

# --- MONITOR CONTINUOUSLY ---

# 8. Monitor RX for 30 seconds
uart_monitor A 10 duration=30000

# 9. Monitor with custom check interval (500ms)
uart_monitor A 10 duration=60000 interval=500

# 10. Monitor with custom baud
uart_monitor A 10 baud=19200 duration=10000

# --- CONFIGURATION & DEBUGGING ---

# 11. View UART pin configuration
uart_config A 9 A 10

# 12. View config with custom baud (shows bit timing)
uart_config A 9 A 10 baud=115200

# ============================================================================
# USAGE PATTERNS
# ============================================================================

# PATTERN 1: Simple Echo Test
# Step 1: Setup TX/RX pins
uart_config A 9 A 10 baud=9600
# Step 2: Send data
uart_send A 9 "Test" baud=9600
# Step 3: Receive response
uart_recv A 10 baud=9600

# PATTERN 2: Sensor Reading
# Connect sensor TX to PA10, configure receiver
uart_recv A 10 timeout=2000 baud=9600 format=hex

# PATTERN 3: Device Communication
# Send command to device on PA9
uart_send A 9 "0x01 0x02 0x03" format=hex baud=115200
# Wait for response on PA10
uart_recv A 10 timeout=1000 baud=115200 format=hex

# PATTERN 4: Long-term Monitoring
# Monitor sensor data for 60 seconds
uart_monitor A 10 duration=60000 interval=100 baud=9600

# PATTERN 5: Multi-Pin Communication
# Device 1: TX on PA9, RX on PA10
uart_config A 9 A 10 baud=9600
uart_send A 9 "Msg1" baud=9600
uart_recv A 10 baud=9600

# Device 2: TX on PB5, RX on PB6
uart_config B 5 B 6 baud=115200
uart_send B 5 "Msg2" baud=115200 format=hex
uart_recv B 6 baud=115200 format=hex

# ============================================================================
# PARAMETER COMBINATIONS
# ============================================================================

# These all work:
uart_send A 9 "Hello"
uart_send A 9 "Hello" baud=19200
uart_send A 9 "Hello" baud=19200 format=ascii
uart_send A 9 "0xFF" format=hex baud=115200

uart_recv A 10
uart_recv A 10 timeout=2000
uart_recv A 10 baud=9600
uart_recv A 10 timeout=5000 baud=115200 format=hex

uart_monitor A 10 duration=10000
uart_monitor A 10 duration=10000 interval=100
uart_monitor A 10 duration=10000 baud=115200

# ============================================================================
# PIN REFERENCE FOR COMMON USES
# ============================================================================

# Connecting to external UART device:
# Device TX -> PA10 (receive on board)
# Device RX <- PA9 (transmit from board)
uart_send A 9 "Data" baud=9600
uart_recv A 10 baud=9600

# Multiple sensors:
# Sensor1 on PA9/PA10
uart_send A 9 "Sensor1_Cmd" baud=9600
# Sensor2 on PB5/PB6
uart_recv B 6 baud=9600

# Using LED pins for debugging (not recommended for production):
# TX on PD12, RX on PD13
uart_send D 12 "Debug" format=hex baud=9600
uart_recv D 13 baud=9600

# ============================================================================
# TROUBLESHOOTING
# ============================================================================

# If receive times out:
#   1. Check baud rate matches
#   2. Verify RX pin is correctly wired
#   3. Increase timeout: uart_recv A 10 timeout=5000

# If data is corrupted:
#   1. Lower baud rate: baud=9600
#   2. Check format matches sender: format=hex or format=ascii
#   3. Verify pin wiring

# To monitor what's happening:
#   1. Use uart_config to verify pins and settings
#   2. Use uart_monitor to see all incoming data
#   3. Start with low baud rates (9600) for testing

# ============================================================================
