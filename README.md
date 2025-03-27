# Solar Tracker with Reaction Wheel and PID Control

## Description

This project implements a single-axis solar tracker that uses Light Dependent Resistors (LDRs) to determine the position of the sun and a reaction wheel mechanism for precise rotation. A PID controller is used to ensure smooth tracking and stability.

## Main Hardware

- Arduino Uno R3
- LDRs (x6)
- Resistors (1kΩ) (x6)
- DC Motor 31ZY (12V 4000RPM)
- L298N Motor Drive
- Power Supply (12V)

## Additional Hardware

- Breadboard
- Jumping Wires
- Lazy Susan Bearing
- Base Plattform (mounted on top of the bearing, containing the Main Hardware)
- 3mm Motor Shaft Coupling (to prevent slipping between the motor shaft and the reaction wheel)
- Reaction Wheel
- Mounting Hardware (screws, brackets zip ties, etc.)

## Wiring

See wiring.png

## Hardware Setup

See setup.png and LDR_setup.png

## Software

The code determines the required motor RPM to align the front facing LDR to face the sun:
- The position of the sun is determined by evaluating the measurements of the LDRs
- The size of the **error** is determind by the angular offset (the further the sun is from the front facing LDR, the greater the error)
- The **integral** is determined by the time the error exists (the longer the integral exist and the greater the error, the greater the integral)
- The **derivative** is determined by the rate at which the error changes (prevents overshooting)
- The PID formula determines the required motor rpm: (Kp * **error**) + (Ki * **integral**) + (Kd * **derivative**)

