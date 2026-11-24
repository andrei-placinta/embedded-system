# Arduino-Based Embedded Control System

An Arduino-based embedded control system developed using **C++ and low-level AVR register programming**.

The project combines serial communication, temperature monitoring, PWM-based LED control, hardware timers, interrupts and a 7-segment display.

## Features

- UART serial communication for receiving commands and controlling LED outputs.
- Analog temperature sensing using the ADC peripheral.
- Temperature-based LED control with configurable threshold and hysteresis.
- PWM-based LED brightness control with fade-in and fade-out states.
- Hardware timer configuration and interrupt-driven operations.
- 7-segment display output for displaying user initials.
- Direct AVR register manipulation for GPIO and peripheral configuration.
- Temperature readings sent through serial output.

## Tech Stack

- **Programming Language:** C++
- **Platform:** Arduino
- **Microcontroller Architecture:** AVR
- **Communication:** UART / USART
- **Analog Input:** ADC
- **Timers:** Timer1 (CTC), Timer2 (Fast PWM)
- **Hardware Features:** GPIO, PWM, Interrupt Service Routines (ISR)
- **Development Environment:** Arduino IDE

## Implementation Details

### UART Communication

- Configured USART communication at 9600 baud.
- Implemented serial data reception and transmission.
- Used serial commands (`A` / `a` and `S` / `s`) to control LED outputs.

### Temperature Monitoring

- Configured the ADC peripheral using the AVcc reference voltage.
- Read analog temperature values from ADC channel 0.
- Converted ADC readings into temperature values.
- Implemented threshold-based LED control using a 35°C threshold and 0.5°C hysteresis.

### Timer and Interrupt Management

- Configured Timer1 in CTC mode for periodic interrupt generation.
- Implemented a Timer1 compare-match interrupt service routine.
- Used Timer2 in Fast PWM mode to control LED brightness.
- Implemented PWM states for fade-in, full brightness, fade-out and off.

### Display and GPIO Control

- Configured AVR GPIO registers directly.
- Controlled output pins through PORTB and PORTD registers.
- Implemented 7-segment display output for displaying user initials.

## Project Structure

```text
embedded-system/
│
├── README.md
└── source.ino
```

## Running Locally

### Requirements

- Arduino IDE
- Compatible AVR-based Arduino board
- Analog temperature sensor
- LEDs
- 7-segment display
- Serial connection

### Setup

1. Install and open **Arduino IDE**.

2. Open `source.ino`.

3. Select an AVR-based Arduino board compatible with the register configuration and 16 MHz clock assumption in the source code.

4. Connect the required components according to the GPIO port configuration and ADC input used in the source code.

5. Select the appropriate serial port.

6. Upload the sketch to the Arduino board.

7. Use the Serial Monitor at 9600 baud to send the supported commands and observe serial output.

## Purpose

This academic project demonstrates embedded C++ programming, low-level AVR register manipulation, microcontroller peripheral configuration, serial communication, analog-to-digital conversion, hardware timers, interrupts and PWM-based control.
