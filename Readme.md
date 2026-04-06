# Flight Simulator

This project simulates a simplified aerospace flight computer system, implementing a closed-loop control system to maintain target altitude using PID control.

## System Overview

The system models key avionics concepts including:

- Sensor data acquisition
- State estimation
- Control algorithms
- Actuator response
- Telemetry logging

## Architecture

- Sensor Module: Simulates altitude, velocity, and acceleration inputs
- Physics Engine: Updates system state based on control outputs
- Controller: PID-based altitude control
- Actuator Module: Applies thrust constraints
- Telemetry System: Logs and exports system state

## Example Output

- Stable altitude hold
- Oscillation under poor PID tuning
- Response to disturbances

## Tech Stack

- C++
- Python (optional visualization)
- CSV / plotting tools

## What I Learned

- Designing real-time simulation loops
- Implementing control systems (PID)
- Handling noisy sensor data
- Structuring modular, testable systems

## Future Improvements

- Kalman filter for state estimation
- Hardware-in-the-loop integration (Arduino/Raspberry Pi)
- Real-time dashboard
