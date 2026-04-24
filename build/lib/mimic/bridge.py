import serial
import serial.tools.list_ports
import time
import logging
from typing import List, Optional, Union

logger = logging.getLogger("Mimic")

class MimicBridge:
    """Core communication class with Auto-Discovery."""
    
    def __init__(self, port: Optional[str] = None, baud: int = 115200, timeout: float = 1.0):
        self.port = port
        self.baud = baud
        self.timeout = timeout
        self.ser = None

    @staticmethod
    def discover_ports() -> List[str]:
        """List all potential serial ports."""
        return [p.device for p in serial.tools.list_ports.comports()]

    def connect(self) -> bool:
        """Connect to the first available Mimic board or a specific port."""
        ports_to_try = [self.port] if self.port else self.discover_ports()
        
        for p in ports_to_try:
            try:
                logger.info(f"Checking {p}...")
                self.ser = serial.Serial(p, self.baud, timeout=2.0)
                self.ser.reset_input_buffer()
                
                # Send a test character to wake up the buffer
                self.ser.write(b"\r\n")
                time.sleep(0.2)
                self.ser.reset_input_buffer()
                
                # Check for Mimic version
                self.send_command("VERSION")
                time.sleep(0.5)  # Give the STM32 more time to respond
                response = self.read_response()
                
                if any("MIMIC" in line.upper() for line in response):
                    self.port = p
                    logger.info(f"Successfully auto-detected Mimic on {p}")
                    return True
                
                self.ser.close()
            except Exception:
                continue
                
        logger.error("No Mimic board found. Check your USB connection.")
        return False

    def disconnect(self):
        if self.ser and self.ser.is_open:
            self.ser.close()

    def send_command(self, cmd: str):
        if self.ser and self.ser.is_open:
            self.ser.write(f"{cmd}\r\n".encode('utf-8'))

    def read_response(self) -> List[str]:
        lines = []
        if not self.ser or not self.ser.is_open:
            return lines
        
        time.sleep(0.1)
        while self.ser.in_waiting:
            line = self.ser.readline().decode('utf-8', errors='ignore').strip()
            if line and line != ">":
                lines.append(line)
        return lines

    def execute(self, cmd: str) -> List[str]:
        self.send_command(cmd)
        return self.read_response()

class I2CSlave:
    """I2C Slave helper."""
    def __init__(self, bridge: MimicBridge, instance: int = 1):
        self.bridge = bridge
        self.instance = instance

    def init(self, address: Union[int, str]) -> bool:
        addr_str = f"0x{address:02X}" if isinstance(address, int) else address
        resp = self.bridge.execute(f"I2C_INIT {self.instance} SLAVE {addr_str}")
        return any("OK" in r for r in resp)

    def wait_for_write(self, length: int = 1) -> Optional[List[int]]:
        resp = self.bridge.execute(f"I2C_READ {self.instance} 0x00 {length}")
        for r in resp:
            if "OK: Read" in r and "bytes:" in r:
                try:
                    data_str = r.split("bytes:")[1].strip()
                    return [int(x, 16) for x in data_str.split()]
                except (IndexError, ValueError):
                    pass
        return None

    def respond(self, data: List[int]) -> bool:
        hex_str = " ".join([f"{x:02X}" for x in data])
        resp = self.bridge.execute(f"I2C_WRITE {self.instance} 0x00 {hex_str}")
        return any("OK" in r for r in resp)
