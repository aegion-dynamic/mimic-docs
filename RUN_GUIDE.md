# Run Guide

This document explains how to run the MIMIC project and the documentation site.

## 1. Documentation (mdBook)

To view the documentation locally:

1. **Install mdBook**:
   ```bash
   cargo install mdbook
   ```
2. **Build and Serve**:
   ```bash
   cd docs-book
   mdbook serve --open
   ```
   This will start a local server at `http://localhost:3000`.

## 2. Project (Mimic CLI)

The Mimic CLI interacts with the STM32 hardware via serial.

1. **Install Dependencies**:
   ```bash
   pip install -e .
   ```
2. **Run Interactive Shell**:
   ```bash
   sudo mimic
   ```
3. **Simulate a Sensor**:
   ```bash
   sudo mimic mpu6050
   ```

## 3. Firmware (STM32)

To compile and flash the firmware to your STM32 board:

1. **Requirements**: `arm-none-eabi-gcc` and `make`.
2. **Build**:
   ```bash
   cd firmware
   make
   ```
3. **Flash**: Use your preferred tool (ST-Link, OpenOCD, etc.) to flash the `build/Mimic.bin` file.

## 4. Hosting the Full Site

The landing page and documentation are now integrated. 

1. **Build the Docs**:
   ```bash
   cd docs-book
   ./mdbook build
   ```
   This builds the book into `landing-page/docs/`.

2. **Run Everything Locally**:
   From the root directory:
   ```bash
   cd landing-page
   python3 -m http.server 8080
   ```
   Open `http://localhost:8080` to see the landing page and navigate to the docs seamlessly.

3. **Deploying to Production**:
   Simply upload the entire `landing-page` folder to your host (Netlify, GitHub Pages, Vercel, etc.). All relative links between the landing page and the docs are already configured.
