"""
Hardware UART Implementation for STM32F411
Uses USART1 on PA9 (TX) and PA10 (RX)
"""

import subprocess
import time

class HardwareUART:
    """Hardware UART using STM32F411 USART peripheral"""
    
    def __init__(self, baud=9600, **kwargs):
        """
        Initialize hardware UART
        
        Args:
            baud: Baud rate (9600, 19200, 38400, 115200, 230400)
            **kwargs: Additional config (ignored, for compatibility)
        """
        self.baud = baud
        self.tx_port = 'A'
        self.tx_pin = 9
        self.rx_port = 'A'
        self.rx_pin = 10
        
        self._configure_usart()
    
    def _run_gdb_command(self, gdb_script):
        """Execute GDB command"""
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
    
    def _configure_usart(self):
        """Configure USART1 on PA9/PA10 at specified baud rate"""
        # Calculate baud rate divider
        # STM32F411 APB2 clock = 84MHz
        # BRR = APB2_CLK / (16 * BAUD)
        baud_div = 84000000 // (16 * self.baud)
        
        gdb_script = f"""
# Enable GPIOA clock
set $rcc = (unsigned int *) 0x40023830
set $rcc_val = *$rcc
set $rcc_val = $rcc_val | (1 << 0)
*$rcc = $rcc_val

# Enable USART1 clock
set $rcc_val = *$rcc
set $rcc_val = $rcc_val | (1 << 4)
*$rcc = $rcc_val

# Configure PA9 (TX) and PA10 (RX) as alternate function 7 (USART1)
set $gpioa = (unsigned int *) 0x40020000
set $moder = $gpioa[0]
set $moder = ($moder & ~(0xF << 18)) | (0xA << 18)
$gpioa[0] = $moder

# Set alternate function
set $afrh = $gpioa[9]
set $afrh = ($afrh & ~(0xFF << 4)) | (0x77 << 4)
$gpioa[9] = $afrh

# Configure USART1
set $usart1 = (unsigned int *) 0x40011000
# BRR (Baud Rate Register)
$usart1[0] = {baud_div}

# CR1 (Control Register 1) - Enable TE, RE, UE
set $cr1 = (1 << 13) | (1 << 3) | (1 << 2)
$usart1[1] = $cr1

printf "USART1 configured at {self.baud} baud\\n"
"""
        self._run_gdb_command(gdb_script)
    
    def send(self, data):
        """Send data via USART1"""
        # Convert data to bytes
        if isinstance(data, str):
            byte_values = [ord(c) for c in data]
        else:
            byte_values = data
        
        gdb_script = "set $usart1 = (unsigned int *) 0x40011000\n"
        
        for byte_val in byte_values:
            gdb_script += f"""
# Wait for TX empty
set $timeout = 1000
while ($timeout > 0 && !($usart1[1] & (1 << 7)))
    set $timeout = $timeout - 1
end

# Send byte
$usart1[2] = {byte_val}
"""
        
        self._run_gdb_command(gdb_script)
        return True
    
    def receive(self, timeout_ms=1000):
        """Receive data from USART1"""
        start_time = time.time()
        data = []
        
        while (time.time() - start_time) * 1000 < timeout_ms:
            gdb_script = """
set $usart1 = (unsigned int *) 0x40011000
set $sr = $usart1[1]
if ($sr & (1 << 5))
    set $data = $usart1[2] & 0xFF
    printf "%c", $data
else
    printf "."
end
"""
            stdout, _ = self._run_gdb_command(gdb_script)
            
            if stdout:
                for char in stdout:
                    if char not in ['.', '\n']:
                        data.append(char)
            
            time.sleep(0.01)
        
        return ''.join(data) if data else None
