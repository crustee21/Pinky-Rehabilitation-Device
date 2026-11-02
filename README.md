# EMG-Controlled Pinky Assist

A wearable prototype that uses muscle activity to control a servo that assists pinky movement. This repository contains the device’s codebase.

## Bill of Materials

- SG90 servo motor
- MyoWare EMG sensor
- Teensy 4.0 microcontroller
- LiPo battery
- TP4056 LiPo charging module
- Boost converter

## Setup and Calibration

Connect the components according to the pin assignments in the code. At startup, relax your arm during EMG calibration so the device can measure a resting baseline.

Servo angles depend on the mounting position and linkage. **Test and adjust the open and closed angles for your setup before attaching the mechanism to your finger.** Start with a small range of motion and ensure the mechanism does not force the finger beyond a comfortable position.

EMG thresholds may also need adjustment for sensor placement and individual muscle activity.
