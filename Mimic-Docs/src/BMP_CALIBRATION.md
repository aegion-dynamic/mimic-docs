# BMP280 Calibration Data

## Overview

The BMP280 sensor requires specific calibration coefficients to convert raw pressure and temperature readings into meaningful engineering units. Mimic emulates these factory-programmed coefficients.

## Calibration Coefficients

Upon power-up, the sensor's read-only memory (0x88 to 0xA1) contains 12 coefficients (`dig_T1` to `dig_P9`).
Mimic provides default values for these coefficients that match a typical BMP280 device.

## Why it Matters

Emulating these coefficients is essential for testing the compensation math in your driver. Without accurate calibration data, the raw pressure values cannot be converted to altitude or weather data correctly.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
