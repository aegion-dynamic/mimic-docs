# GPS Trajectory Simulation

## Overview

Mimic can simulate movement by following a pre-defined trajectory. This is useful for testing geofencing, navigation algorithms, or route tracking software.

## Trajectory Formats

The Python bridge supports loading trajectories from:
- **CSV Files:** A simple list of Latitude, Longitude, and Altitude.
- **KML/GPX Files:** Standard map route formats.
- **Dynamic Generation:** Real-time generation of paths (e.g., circular or random walk) via Python scripts.

## Real-Time Updates

As the simulation progresses, the Python bridge calculates the current position and sends the corresponding coordinates to the Mimic hardware. The hardware then packages these into NMEA sentences for the UART interface.

---
*© [Aegion Dynamic](https://aegiondynamic.com)*
