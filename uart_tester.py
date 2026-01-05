#!/usr/bin/env python3
"""
Simple UART Testing Tool for Arduino + STM32
Communicates with both boards via USB serial
"""

import serial
import time
import sys
import argparse

class UARTTester:
    def __init__(self, arduino_port="/dev/ttyUSB0", stm32_port="/dev/ttyUSB1", baud=9600):
        """
        Initialize UART tester
        
        Args:
            arduino_port: Serial port for Arduino
            stm32_port: Serial port for STM32
            baud: Baud rate (default 9600)
        """
        self.baud = baud
        self.arduino = None
        self.stm32 = None
        self.arduino_port = arduino_port
        self.stm32_port = stm32_port
    
    def connect_arduino(self):
        """Connect to Arduino"""
        try:
            self.arduino = serial.Serial(self.arduino_port, self.baud, timeout=2)
            time.sleep(1)  # Wait for Arduino to reset
            
            # Read startup message
            startup = self.arduino.readline()
            print(f"[Arduino] {startup.decode('utf-8', errors='ignore').strip()}")
            return True
        except Exception as e:
            print(f"Error connecting to Arduino: {e}")
            return False
    
    def connect_stm32(self):
        """Connect to STM32"""
        try:
            self.stm32 = serial.Serial(self.stm32_port, self.baud, timeout=2)
            time.sleep(1)  # Wait for STM32 startup
            
            # Read startup message
            startup = self.stm32.readline()
            print(f"[STM32] {startup.decode('utf-8', errors='ignore').strip()}")
            return True
        except Exception as e:
            print(f"Error connecting to STM32: {e}")
            return False
    
    def send_to_arduino(self, data):
        """Send data to Arduino"""
        if not self.arduino:
            print("Arduino not connected")
            return False
        
        try:
            self.arduino.write((data + '\n').encode('utf-8'))
            self.arduino.flush()
            print(f"→ Arduino: {data}")
            
            # Read response
            time.sleep(0.1)
            response = self.arduino.read(100)
            if response:
                print(f"← Arduino: {response.decode('utf-8', errors='ignore').strip()}")
            return True
        except Exception as e:
            print(f"Error sending to Arduino: {e}")
            return False
    
    def send_to_stm32(self, data):
        """Send data to STM32"""
        if not self.stm32:
            print("STM32 not connected")
            return False
        
        try:
            self.stm32.write((data + '\n').encode('utf-8'))
            self.stm32.flush()
            print(f"→ STM32: {data}")
            
            # Read response
            time.sleep(0.1)
            response = self.stm32.read(100)
            if response:
                print(f"← STM32: {response.decode('utf-8', errors='ignore').strip()}")
            return True
        except Exception as e:
            print(f"Error sending to STM32: {e}")
            return False
    
    def test_arduino(self):
        """Test Arduino communication"""
        print("\n=== Testing Arduino ===")
        self.send_to_arduino("PING")
        time.sleep(0.5)
        self.send_to_arduino("ID")
        time.sleep(0.5)
        self.send_to_arduino("STATUS")
    
    def test_stm32(self):
        """Test STM32 communication"""
        print("\n=== Testing STM32 ===")
        self.send_to_stm32("PING")
        time.sleep(0.5)
        self.send_to_stm32("ID")
        time.sleep(0.5)
        self.send_to_stm32("STATUS")
    
    def interactive_mode(self):
        """Interactive mode for manual testing"""
        print("\n=== Interactive Mode ===")
        print("Commands:")
        print("  arduino <message>  - Send to Arduino")
        print("  stm32 <message>    - Send to STM32")
        print("  test               - Run tests")
        print("  exit               - Exit")
        print()
        
        while True:
            try:
                cmd = input("> ").strip()
                
                if cmd.startswith("arduino "):
                    msg = cmd[8:]
                    self.send_to_arduino(msg)
                elif cmd.startswith("stm32 "):
                    msg = cmd[6:]
                    self.send_to_stm32(msg)
                elif cmd == "test":
                    self.test_arduino()
                    self.test_stm32()
                elif cmd == "exit":
                    break
                else:
                    print("Unknown command")
            except KeyboardInterrupt:
                break
            except Exception as e:
                print(f"Error: {e}")
    
    def close(self):
        """Close connections"""
        if self.arduino:
            self.arduino.close()
        if self.stm32:
            self.stm32.close()


def main():
    parser = argparse.ArgumentParser(
        description="Simple UART Testing Tool for Arduino + STM32",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Test Arduino and STM32
  python3 uart_tester.py --test
  
  # Interactive mode
  python3 uart_tester.py --interactive
  
  # Custom ports
  python3 uart_tester.py --arduino /dev/ttyUSB0 --stm32 /dev/ttyUSB1 --test
  
  # Custom baud rate
  python3 uart_tester.py --baud 19200 --test
        """
    )
    
    parser.add_argument("--arduino", default="/dev/ttyUSB0", 
                       help="Arduino serial port (default: /dev/ttyUSB0)")
    parser.add_argument("--stm32", default="/dev/ttyUSB1",
                       help="STM32 serial port (default: /dev/ttyUSB1)")
    parser.add_argument("--baud", type=int, default=9600,
                       help="Baud rate (default: 9600)")
    parser.add_argument("--test", action="store_true",
                       help="Run automated tests")
    parser.add_argument("--interactive", action="store_true",
                       help="Interactive mode")
    
    args = parser.parse_args()
    
    tester = UARTTester(args.arduino, args.stm32, args.baud)
    
    # Try to connect to both
    print(f"Connecting at {args.baud} baud...")
    arduino_ok = tester.connect_arduino()
    stm32_ok = tester.connect_stm32()
    
    if not arduino_ok and not stm32_ok:
        print("Error: Could not connect to either board")
        return 1
    
    if args.test:
        if arduino_ok:
            tester.test_arduino()
        if stm32_ok:
            tester.test_stm32()
    elif args.interactive or (not args.test):
        tester.interactive_mode()
    
    tester.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
