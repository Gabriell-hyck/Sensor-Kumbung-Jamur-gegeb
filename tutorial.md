# kumbung jamur.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Repository overview

This repository contains a single Arduino sketch, `penyiraman_otomatis_suhu.ino`, implementing an automatic plant-watering system controlled by temperature.

There is no separate README, test suite, or additional tooling configuration; this file is the primary place for project-specific guidance.

## Development and build commands

This is a standard Arduino sketch. You can work with it using either the Arduino IDE or `arduino-cli`.

### Using the Arduino IDE

- Open the project directory in the Arduino IDE and load `penyiraman_otomatis_suhu.ino`.
- Select the correct **Board** and **Port** for your hardware.
- To build (verify/compile) the sketch:
  - Use **Sketch → Verify/Compile** in the IDE.
- To upload the sketch to the board:
  - Use **Sketch → Upload**.
- To view serial output (temperature readings and pump state):
  - Open **Tools → Serial Monitor** and set the baud rate to `9600`.

### Using arduino-cli

These commands assume you have `arduino-cli` installed and your board fully qualified board name (FQBN) and serial port are known.

Set environment variables (recommended so you can reuse them):

```bash path=null start=null
$env:ARDUINO_FQBN = "arduino:avr:uno"   # Example for Arduino Uno; adjust to your board
$env:ARDUINO_PORT = "COM3"              # Adjust to your actual serial port
```

Compile the sketch:

```bash path=null start=null
arduino-cli compile --fqbn $env:ARDUINO_FQBN .
```

Upload the compiled sketch to the board:

```bash path=null start=null
arduino-cli upload --fqbn $env:ARDUINO_FQBN --port $env:ARDUINO_PORT .
```

Monitor serial output at 9600 baud:

```bash path=null start=null
arduino-cli monitor -p $env:ARDUINO_PORT -c baudrate=9600
```

### Linting and tests

- There is no project-specific linting configuration or automated test suite defined in this repository.
- Rely on the Arduino compiler output for basic static checking and use a physical setup (sensor, relay, and pump) for behavior verification.

## Code architecture and structure

### High-level behavior

The sketch continuously reads temperature from an analog sensor, converts the reading into degrees Celsius, logs diagnostic information over the serial port, and switches a relay-controlled pump on or off depending on a configurable temperature threshold.

Hardware assumptions are:
- An analog temperature sensor (e.g., LM35) connected to `A0`.
- A relay or pump driver connected to digital pin `D8`.
- A 5V reference and a 10-bit ADC (standard on many Arduino boards).

### Key components

All logic is contained within `penyiraman_otomatis_suhu.ino`:

- **Global configuration and pins**
  - `SENSOR_SUHU_PIN` (A0): analog input for the temperature sensor.
  - `RELAY_POMPA_PIN` (8): digital output controlling the relay/pump.
  - `BATAS_SUHU`: temperature threshold in °C (default `30.0`). Pump turns on above this value.

- **`setup()`**
  - Initializes the serial port at `9600` baud for logging.
  - Configures the sensor pin as `INPUT` and relay pin as `OUTPUT`.
  - Ensures the pump is off at startup by writing `LOW` to `RELAY_POMPA_PIN`.

- **`loop()`**
  - Reads the raw analog value from the temperature sensor.
  - Converts the raw ADC value to a voltage assuming a 0–5V range and 10-bit resolution.
  - Converts voltage to temperature using LM35 characteristics (`10 mV/°C`, or `0.01 V/°C`).
  - Logs raw ADC value, voltage, and computed temperature to the serial console.
  - Compares the computed temperature against `BATAS_SUHU`:
    - If `suhu > BATAS_SUHU`, sets `RELAY_POMPA_PIN` to `HIGH` and logs that the pump is on.
    - Otherwise, sets `RELAY_POMPA_PIN` to `LOW` and logs that the pump is off.
  - Waits 1 second (`delay(1000)`) between measurements to limit noise and serial spam.

### Important implementation details

- Temperature conversion assumes:
  - ADC reference voltage is exactly 5.0V.
  - Sensor is an LM35 or another sensor with the same `10 mV/°C` linear relationship.
- Changing the sensor type or reference voltage will require updating the conversion formula in `loop()`.
- Adjusting `BATAS_SUHU` is the primary way to tune when the pump turns on/off without changing code structure.
