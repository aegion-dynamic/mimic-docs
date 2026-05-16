# Troubleshooting

## Common Issues

### 1. Board Not Detected
- **Symptoms:** The CLI returns `No device found` or `Access Denied`.
- **Solutions:**
  - Check the USB-C cable connection.
  - Ensure you have the correct permissions to access the serial port (on Linux, check if your user is in the `dialout` group).
  - Try specifying the port explicitly using `--port`.

### 2. Protocol Timeouts
- **Symptoms:** I2C or SPI transactions fail with a "Timeout" error.
- **Solutions:**
  - Verify that the Master device and Mimic share a common Ground (GND).
  - Check the wiring between the Master device and the BlackPill pins.
  - Reduce the bus speed on the Master device (e.g., lower the I2C frequency to 100kHz).

### 3. Firmware Flashing Fails
- **Symptoms:** `make flash` returns an error or fails to connect to the ST-Link.
- **Solutions:**
  - Ensure the ST-Link is properly connected to the SWD pins (3.3V, GND, SWDIO, SWCLK).
  - Check that the ST-Link drivers are installed correctly on your host machine.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
