"""
UART Protocol Implementation via GPIO Bit-Banging
Supports configurable baud rate, data format, and frame structure
"""

import time
import subprocess
from protocols.base import BaseProtocol

# STM32F411 GPIO Port base addresses
PORTS = {
    "A": {"base": 0x40020000, "rcc_bit": 0},
    "B": {"base": 0x40020400, "rcc_bit": 1},
    "C": {"base": 0x40020800, "rcc_bit": 2},
    "D": {"base": 0x40020C00, "rcc_bit": 3},
    "E": {"base": 0x40021000, "rcc_bit": 4},
}

# Register offsets
GPIO_MODER = 0x00
GPIO_IDR = 0x10
GPIO_ODR = 0x14
GPIO_BSRR = 0x18
RCC_AHB1ENR = 0x40023830

class UART(BaseProtocol):
    """UART protocol using GPIO bit-banging"""
    
    def __init__(self, tx_port, tx_pin, rx_port, rx_pin, **kwargs):
        """
        Initialize UART with TX and RX pins
        
        Args:
            tx_port: TX port (A-E)
            tx_pin: TX pin number (0-15)
            rx_port: RX port (A-E)
            rx_pin: RX pin number (0-15)
            **kwargs: Configuration options:
                - baud: Baud rate (9600, 19200, 38400, 57600, 115200) [default: 9600]
                - data_format: 'ascii' or 'hex' [default: 'ascii']
                - frame: 'standard' (1 start, 8 data, 1 stop) [default: 'standard']
                - timeout: Receive timeout in ms [default: 1000]
        """
        super().__init__(**kwargs)
        
        self.tx_port = tx_port.upper()
        self.tx_pin = tx_pin
        self.rx_port = rx_port.upper()
        self.rx_pin = rx_pin
        
        # Validate ports and pins
        if self.tx_port not in PORTS:
            raise ValueError(f"Invalid TX port: {self.tx_port}")
        if self.rx_port not in PORTS:
            raise ValueError(f"Invalid RX port: {self.rx_port}")
        if not (0 <= self.tx_pin <= 15):
            raise ValueError(f"Invalid TX pin: {self.tx_pin}")
        if not (0 <= self.rx_pin <= 15):
            raise ValueError(f"Invalid RX pin: {self.rx_pin}")
        
        # Configuration with defaults
        self.baud = kwargs.get('baud', 9600)
        self.data_format = kwargs.get('data_format', 'ascii')  # 'ascii' or 'hex'
        self.frame = kwargs.get('frame', 'standard')  # 'standard' or custom
        self.timeout = kwargs.get('timeout', 1000)
        
        # Validate baud rate
        valid_bauds = [9600, 19200, 38400, 57600, 115200, 230400]
        if self.baud not in valid_bauds:
            raise ValueError(f"Invalid baud rate: {self.baud}. Valid: {valid_bauds}")
        
        # Validate data format
        if self.data_format not in ['ascii', 'hex']:
            raise ValueError(f"Invalid data format: {self.data_format}. Valid: ascii, hex")
        
        # Calculate bit time in seconds
        self.bit_time = 1.0 / self.baud
        
        # Initialize GPIO pins
        self._configure_pins()
    
    def _run_gdb_command(self, gdb_script):
        """Execute GDB command for low-level access"""
        cmd = ["arm-none-eabi-gdb", "-q", "-batch"]
        cmd.extend(["-ex", "set confirm off"])
        cmd.extend(["-ex", "target extended-remote :3333"])
        
        for line in gdb_script.strip().split('\n'):
            line = line.strip()
            if line:
                cmd.extend(["-ex", line])
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=10
            )
            return result.stdout, result.stderr
        except Exception as e:
            return "", str(e)
    
    def _configure_pins(self):
        """Configure TX as output and RX as input"""
        tx_base = PORTS[self.tx_port]["base"]
        rx_base = PORTS[self.rx_port]["base"]
        
        # GDB script to configure pins
        gdb_script = f"""
set $rcc = (unsigned int *) {RCC_AHB1ENR}
set $tx_port = (unsigned int *) {tx_base}
set $rx_port = (unsigned int *) {rx_base}

# Enable clocks for TX and RX ports
set $rcc_val = *$rcc
set $rcc_val = $rcc_val | (1 << {PORTS[self.tx_port]["rcc_bit"]})
set $rcc_val = $rcc_val | (1 << {PORTS[self.rx_port]["rcc_bit"]})
*$rcc = $rcc_val

# Configure TX pin as output (MODER)
set $moder = $tx_port[{GPIO_MODER // 4}]
set $moder = $moder | (1 << {self.tx_pin * 2})
$tx_port[{GPIO_MODER // 4}] = $moder

# Configure RX pin as input (MODER bits = 0)
set $moder = $rx_port[{GPIO_MODER // 4}]
set $moder = $moder & ~(3 << {self.rx_pin * 2})
$rx_port[{GPIO_MODER // 4}] = $moder

printf "UART pins configured: TX={self.tx_port}{self.tx_pin}, RX={self.rx_port}{self.rx_pin}\\n"
"""
        self._run_gdb_command(gdb_script)
    
    def _set_tx_high(self):
        """Set TX pin HIGH"""
        tx_base = PORTS[self.tx_port]["base"]
        # BSRR register: write 1 to set bit (upper 16 bits)
        bsrr_val = (1 << (self.tx_pin + 16))
        gdb_script = f"""
set $tx_port = (unsigned int *) {tx_base}
$tx_port[{GPIO_BSRR // 4}] = {hex(bsrr_val)}
"""
        self._run_gdb_command(gdb_script)
    
    def _set_tx_low(self):
        """Set TX pin LOW"""
        tx_base = PORTS[self.tx_port]["base"]
        # BSRR register: write 1 to lower 16 bits to reset
        bsrr_val = (1 << self.tx_pin)
        gdb_script = f"""
set $tx_port = (unsigned int *) {tx_base}
$tx_port[{GPIO_BSRR // 4}] = {hex(bsrr_val)}
"""
        self._run_gdb_command(gdb_script)
    
    def _read_rx_pin(self):
        """Read RX pin value"""
        rx_base = PORTS[self.rx_port]["base"]
        gdb_script = f"""
set $rx_port = (unsigned int *) {rx_base}
set $idr = $rx_port[{GPIO_IDR // 4}]
set $bit = ($idr >> {self.rx_pin}) & 1
printf "%d\\n", $bit
"""
        stdout, stderr = self._run_gdb_command(gdb_script)
        
        # Extract the bit value from output
        for line in stdout.split('\n'):
            if line.strip() in ['0', '1']:
                return int(line.strip())
        return 0
    
    def _transmit_bit(self, bit_value):
        """Transmit a single bit"""
        if bit_value:
            self._set_tx_high()
        else:
            self._set_tx_low()
        
        # Wait for bit time (with some tolerance)
        time.sleep(self.bit_time * 0.95)  # Reduced for GDB overhead
    
    def _receive_bit(self):
        """Receive a single bit with sampling at mid-bit"""
        # Wait for bit start
        time.sleep(self.bit_time * 0.5)
        # Sample at mid-bit
        bit = self._read_rx_pin()
        # Wait for rest of bit
        time.sleep(self.bit_time * 0.5)
        return bit
    
    def send(self, data):
        """
        Send data via UART
        
        Args:
            data: String (ascii format) or hex string (hex format)
                  Examples: "Hello" or "0x48 0x65 0x6C 0x6C 0x6F"
        
        Returns:
            bool: True if successful
        """
        try:
            # Convert data to bytes
            if self.data_format == 'hex':
                # Parse hex format: "0xAA 0xBB 0xCC" or "AA BB CC"
                data = data.replace('0x', '').replace('0X', '')
                byte_values = []
                for part in data.split():
                    if part:
                        byte_values.append(int(part, 16))
            else:  # ascii
                # Convert string to bytes
                byte_values = [ord(c) for c in data]
            
            # Send each byte with UART frame (1 start, 8 data, 1 stop)
            for byte_val in byte_values:
                self._send_byte(byte_val)
            
            return True
        except Exception as e:
            print(f"Send error: {e}")
            return False
    
    def _send_byte(self, byte_val):
        """Send a single byte with UART frame"""
        # Start bit (LOW)
        self._transmit_bit(0)
        
        # Data bits (LSB first)
        for i in range(8):
            bit = (byte_val >> i) & 1
            self._transmit_bit(bit)
        
        # Stop bit (HIGH)
        self._transmit_bit(1)
    
    def receive(self, timeout_ms=None):
        """
        Receive data via UART
        
        Args:
            timeout_ms: Timeout in milliseconds (uses self.timeout if None)
        
        Returns:
            str: Received data (formatted according to data_format)
        """
        timeout_ms = timeout_ms or self.timeout
        start_time = time.time()
        received_bytes = []
        
        try:
            while (time.time() - start_time) * 1000 < timeout_ms:
                # Look for start bit (LOW)
                if self._read_rx_pin() == 0:
                    # Found start bit, wait for data bits
                    time.sleep(self.bit_time * 1.5)
                    
                    # Read 8 data bits
                    byte_val = 0
                    for i in range(8):
                        bit = self._receive_bit()
                        byte_val |= (bit << i)
                    
                    # Read stop bit (should be HIGH)
                    stop_bit = self._receive_bit()
                    
                    if stop_bit == 1:
                        received_bytes.append(byte_val)
                    
                    # Short delay before looking for next byte
                    time.sleep(self.bit_time)
                else:
                    # No start bit yet, keep waiting
                    time.sleep(0.001)
            
            # Format output
            if self.data_format == 'hex':
                return ' '.join([f"0x{b:02X}" for b in received_bytes])
            else:  # ascii
                return ''.join([chr(b) for b in received_bytes])
        
        except Exception as e:
            print(f"Receive error: {e}")
            return None
    
    def monitor(self, duration_ms=10000, interval_ms=100):
        """
        Monitor RX for incoming data
        
        Args:
            duration_ms: Duration to monitor
            interval_ms: Check interval
        
        Returns:
            list: List of (timestamp, data) tuples
        """
        results = []
        start_time = time.time()
        
        try:
            while (time.time() - start_time) * 1000 < duration_ms:
                data = self.receive(timeout_ms=interval_ms)
                if data:
                    timestamp = time.time() - start_time
                    results.append((timestamp, data))
                    print(f"[{timestamp:.2f}ms] RX: {data}")
                
                time.sleep(interval_ms / 1000.0)
            
            return results
        except KeyboardInterrupt:
            print("\nMonitoring stopped")
            return results
    
    def get_config(self):
        """Return current configuration"""
        return {
            'tx_pin': f"{self.tx_port}{self.tx_pin}",
            'rx_pin': f"{self.rx_port}{self.rx_pin}",
            'baud': self.baud,
            'data_format': self.data_format,
            'frame': self.frame,
            'timeout': self.timeout,
            'bit_time': self.bit_time
        }
