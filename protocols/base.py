"""
Base Protocol Class - Abstract interface for communication protocols
Supports UART, SPI, I2C implementations
"""

class BaseProtocol:
    """Abstract base class for all communication protocols"""
    
    def __init__(self, **kwargs):
        """
        Initialize protocol with configuration
        
        Args:
            **kwargs: Protocol-specific parameters (baud rate, pins, etc.)
        """
        self.config = kwargs
    
    def send(self, data):
        """
        Send data over the protocol
        
        Args:
            data: Data to send (string, bytes, or hex format)
            
        Returns:
            bool: True if successful, False otherwise
        """
        raise NotImplementedError("Subclass must implement send()")
    
    def receive(self, timeout_ms=1000):
        """
        Receive data from the protocol
        
        Args:
            timeout_ms: Timeout in milliseconds
            
        Returns:
            bytes: Received data, None if timeout
        """
        raise NotImplementedError("Subclass must implement receive()")
    
    def monitor(self, duration_ms=10000, interval_ms=100):
        """
        Continuously monitor for incoming data
        
        Args:
            duration_ms: Duration to monitor in milliseconds
            interval_ms: Check interval in milliseconds
            
        Returns:
            list: List of (timestamp, data) tuples
        """
        raise NotImplementedError("Subclass must implement monitor()")
    
    def close(self):
        """Clean up resources"""
        pass
