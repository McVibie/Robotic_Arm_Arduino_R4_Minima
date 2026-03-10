# Robotic Arm Arduino R4 Minima

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Arduino-orange)](https://platformio.org/)
[![Board](https://img.shields.io/badge/Board-UNO%20R4%20Minima-00979D)](https://docs.platformio.org/en/latest/boards/renesas-ra/uno_r4_minima.html)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Bluetooth-controlled robotic arm firmware for **Arduino UNO R4 Minima**, built with PlatformIO and controlled from a **MIT App Inventor** Bluetooth app.

## Features
- 4 servo axes controlled by Bluetooth commands (`s1` to `s4`)
- Adjustable movement speed with `speed` command
- DC gripper control with `s5` command
- MIT App Inventor app support

## Project Structure
- `src/main.cpp` - firmware source code
- `platformio.ini` - PlatformIO board/environment config
- `docs/images/mit-app/` - MIT App Inventor screenshots
- `docs/images/hardware/` - hardware/wiring images
- `docs/images/demo/` - robot demo image

## How To Use
1. Open this folder in PlatformIO.
2. Build firmware:
   ```bash
   platformio run
   ```
3. Upload firmware:
   ```bash
   platformio run --target upload
   ```
4. Pair your phone with the HC-05/HC-06 Bluetooth module.
5. Open the MIT App Inventor app and connect to the module.
6. Move sliders/buttons in the app to send commands.

## Bluetooth Command Protocol
- `s1<0-180>` -> Servo 1 angle
- `s2<0-180>` -> Servo 2 angle
- `s3<0-180>` -> Servo 3 angle
- `s4<0-180>` -> Servo 4 angle
- `speed<1-100>` -> delay (ms) per movement step
- `s5<0-180>` -> gripper logic (`>100` open, `<80` close, otherwise stop)

Examples:
- `s190`
- `s245`
- `speed20`
- `s5120`

## Pinout
| Function | Arduino Pin | Notes |
|---|---:|---|
| Servo 1 signal | D5 | Base axis |
| Servo 2 signal | D6 | Shoulder axis |
| Servo 3 signal | D7 | Elbow axis |
| Servo 4 signal | D8 | Wrist axis |
| Gripper IN1 | D9 | Motor direction control |
| Gripper IN2 | D10 | Motor direction control |
| Gripper EN (PWM) | D11 | Speed/enable (analogWrite) |
| Bluetooth RX (SoftwareSerial TX) | D3 | Connect to BT module TX |
| Bluetooth TX (SoftwareSerial RX) | D4 | Connect to BT module RX (use level shifting if needed) |

## MIT App Inventor (Bluetooth App)
![MIT App Inventor Blocks](docs/images/mit-app/blocks.png)
![MIT App Inventor App Screen](docs/images/mit-app/app-screen.png)

## Hardware
![Hardware Scheme](docs/images/hardware/scheme.jpg)

## Demo
![Robotic Arm Demo](docs/images/demo/robotic-arm.png)

## Notes
- Firmware drives servos without `Servo.h` (manual pulse generation).
- Bluetooth serial is configured on pins `3` (RX) and `4` (TX).

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md).
