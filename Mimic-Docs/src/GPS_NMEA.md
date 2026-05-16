# GPS Module (NMEA)

## Overview

Mimic emulates a standard GPS receiver by streaming NMEA 0183 sentences over a UART interface. This allows you to test GPS parsing libraries and navigation software using simulated or pre-recorded trajectories.

## Supported Sentences

The emulation module can generate the following standard NMEA messages:
- **$GPRMC:** Recommended Minimum Specific GNSS Data.
- **$GPGGA:** Global Positioning System Fix Data.
- **$GPGSV:** GNSS Satellites in View.
- **$GPGLL:** Geographic Position - Latitude/Longitude.

## Trajectory Simulation

You can provide a CSV file containing a list of coordinates, and Mimic will "travel" along that path, updating the NMEA stream in real-time to match the simulated movement.

## Usage

Start the GPS emulation on the default UART pins (**PA9/PA10**):
```bash
mimic simulate gps --trajectory my_trajectory.csv
```

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
