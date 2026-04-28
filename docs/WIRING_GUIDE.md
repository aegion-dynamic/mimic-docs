# Mimic Wiring Guide

This guide ensures reliable electrical connection between Mimic (STM32) and your User Device (ESP8266/Arduino).

## Ground Rule #1: Shared Ground
**CRITICAL**: You MUST connect a GND pin on the STM32 to a GND pin on the User Device. Without this, signal levels will float and data will be corrupted.

---

## 1. MPU6050 (I2C) Wiring
The MPU6050 is emulated on I2C Instance 1.

| STM32 Pin | Function | User Device Pin |
| :--- | :--- | :--- |
| **PB6** | SCL (Clock) | SCL (ESP8266 D1 / GPIO5) |
| **PB7** | SDA (Data) | SDA (ESP8266 D2 / GPIO4) |
| **GND** | Ground | GND |

**Note on Resistors**: Our firmware is optimized to work without resistors at 100kHz, but if you experience "Bus Errors," add a 4.7kΩ resistor from SDA to 3.3V and SCL to 3.3V.

---

## 2. BMP280 (I2C) Wiring
The BMP280 uses the same pins as the MPU6050 but at a different address (0x76).

| STM32 Pin | Function | User Device Pin |
| :--- | :--- | :--- |
| **PB6** | SCL (Clock) | SCL |
| **PB7** | SDA (Data) | SDA |
| **GND** | Ground | GND |

---

## 3. General SPI Wiring (Preview)
When emulated, SPI follows this standard mapping:

| STM32 Pin | Function | User Device Pin |
| :--- | :--- | :--- |
| **PA5** | SCK (Clock) | SCK |
| **PA6** | MISO (Out) | MISO |
| **PA7** | MOSI (In) | MOSI |
| **PA4** | CS (Select) | CS / SS |
| **GND** | Ground | GND |
