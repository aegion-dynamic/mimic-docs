#!/usr/bin/env python3
"""
Test UART Protocol Implementation
Validates command parsing and configuration
"""

import sys
sys.path.insert(0, '/home/karthik/Documents/Aegion/Mimic')

from protocols.uart import UART

def test_uart_initialization():
    """Test UART object creation with various parameters"""
    print("Test 1: UART Initialization")
    
    try:
        # Test with defaults
        uart1 = UART('A', 9, 'A', 10)
        config = uart1.get_config()
        print(f"  ✓ Default config: {config}")
        assert config['baud'] == 9600
        assert config['data_format'] == 'ascii'
        
        # Test with custom baud
        uart2 = UART('B', 5, 'B', 6, baud=115200)
        config2 = uart2.get_config()
        print(f"  ✓ Custom baud: {config2['baud']}")
        assert config2['baud'] == 115200
        
        # Test with hex format
        uart3 = UART('C', 0, 'C', 1, data_format='hex')
        config3 = uart3.get_config()
        print(f"  ✓ Hex format: {config3['data_format']}")
        assert config3['data_format'] == 'hex'
        
        print("  ✓ All initialization tests passed\n")
        return True
    except Exception as e:
        print(f"  ✗ Error: {e}\n")
        return False

def test_invalid_ports():
    """Test validation of invalid ports"""
    print("Test 2: Invalid Port Validation")
    
    try:
        uart = UART('Z', 9, 'A', 10)
        print(f"  ✗ Should have rejected invalid port 'Z'\n")
        return False
    except ValueError as e:
        print(f"  ✓ Correctly rejected: {e}\n")
        return True

def test_invalid_pins():
    """Test validation of invalid pins"""
    print("Test 3: Invalid Pin Validation")
    
    try:
        uart = UART('A', 20, 'A', 10)
        print(f"  ✗ Should have rejected invalid pin 20\n")
        return False
    except ValueError as e:
        print(f"  ✓ Correctly rejected: {e}\n")
        return True

def test_invalid_baud():
    """Test validation of invalid baud rates"""
    print("Test 4: Invalid Baud Rate Validation")
    
    try:
        uart = UART('A', 9, 'A', 10, baud=12345)
        print(f"  ✗ Should have rejected invalid baud 12345\n")
        return False
    except ValueError as e:
        print(f"  ✓ Correctly rejected: {e}\n")
        return True

def test_invalid_format():
    """Test validation of data format"""
    print("Test 5: Invalid Data Format Validation")
    
    try:
        uart = UART('A', 9, 'A', 10, data_format='binary')
        print(f"  ✗ Should have rejected invalid format 'binary'\n")
        return False
    except ValueError as e:
        print(f"  ✓ Correctly rejected: {e}\n")
        return True

def test_all_valid_bauds():
    """Test all valid baud rates"""
    print("Test 6: All Valid Baud Rates")
    
    valid_bauds = [9600, 19200, 38400, 57600, 115200, 230400]
    
    for baud in valid_bauds:
        try:
            uart = UART('A', 9, 'A', 10, baud=baud)
            config = uart.get_config()
            print(f"  ✓ Baud {baud}: bit_time={config['bit_time']*1000:.4f}ms")
        except Exception as e:
            print(f"  ✗ Failed to create UART with baud {baud}: {e}")
            return False
    
    print()
    return True

def test_pin_combinations():
    """Test various valid port/pin combinations"""
    print("Test 7: Various Port/Pin Combinations")
    
    combinations = [
        ('A', 0, 'A', 1),
        ('A', 15, 'B', 15),
        ('D', 12, 'D', 13),
        ('E', 0, 'E', 15),
    ]
    
    for tx_port, tx_pin, rx_port, rx_pin in combinations:
        try:
            uart = UART(tx_port, tx_pin, rx_port, rx_pin)
            config = uart.get_config()
            print(f"  ✓ TX={config['tx_pin']}, RX={config['rx_pin']}")
        except Exception as e:
            print(f"  ✗ Failed: {e}")
            return False
    
    print()
    return True

def test_configuration_options():
    """Test all configuration options"""
    print("Test 8: Configuration Options")
    
    try:
        uart = UART(
            'A', 9, 'A', 10,
            baud=115200,
            data_format='hex',
            frame='standard',
            timeout=2000
        )
        config = uart.get_config()
        
        print(f"  ✓ Baud: {config['baud']}")
        print(f"  ✓ Format: {config['data_format']}")
        print(f"  ✓ Frame: {config['frame']}")
        print(f"  ✓ Timeout: {config['timeout']}")
        print()
        
        return True
    except Exception as e:
        print(f"  ✗ Error: {e}\n")
        return False

def main():
    """Run all tests"""
    print("\n" + "="*60)
    print("UART Protocol Implementation - Unit Tests")
    print("="*60 + "\n")
    
    tests = [
        test_uart_initialization,
        test_invalid_ports,
        test_invalid_pins,
        test_invalid_baud,
        test_invalid_format,
        test_all_valid_bauds,
        test_pin_combinations,
        test_configuration_options,
    ]
    
    results = []
    for test in tests:
        results.append(test())
    
    passed = sum(results)
    total = len(results)
    
    print("="*60)
    print(f"Results: {passed}/{total} tests passed")
    print("="*60 + "\n")
    
    return all(results)

if __name__ == "__main__":
    success = main()
    sys.exit(0 if success else 1)
