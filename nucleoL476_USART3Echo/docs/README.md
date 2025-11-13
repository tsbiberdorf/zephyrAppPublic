# Nucleo-L476RG USART3 Echo with LED Control

## Overview

This application demonstrates USART3 echo functionality combined with LED control on the STM32 Nucleo-L476RG development board. It includes:

- **LED Control**: Same LED control functionality from the blinky app (on/off/flash modes)
- **USART3 Echo**: Echo characters received on USART3 (PC3/PC4 pins)
- **Shell Commands**: Interactive shell interface for LED control
- **Extensible Design**: UART module designed to be expanded for command parsing in the future

## Hardware Requirements

- STM32 Nucleo-L476RG development board
- USB cable for power and shell interface
- Serial/UART connection to PC3 (RX) and PC4 (TX) for USART3 echo

## Pin Configuration

### USART3 Pins (Custom Overlay)
- **PC3**: USART3_RX (Receive)
- **PC4**: USART3_TX (Transmit)
- **Baud Rate**: 115200

### LED
- **PA5**: Green user LED (LD2)

## Features

### 1. LED Control
The application provides the same LED control functionality as the blinky app:
- Turn LED on/off
- Flash LED with configurable period
- Default mode: Flashing at 1000ms

### 2. USART3 Echo
- Characters received on USART3 are echoed back
- Interrupt-driven UART reception
- Ring buffer for received data (256 bytes)
- Dedicated task for UART processing
- Ready for future command parsing expansion

### 3. Shell Interface
Interactive shell commands available via the console:
- `led on` - Turn LED on
- `led off` - Turn LED off
- `led flash [ms]` - Flash LED with optional period (100-10000 ms)
- `led status` - Show current LED status
- `version` - Display application version

## Building and Flashing

### Build the application
```bash
west build -b nucleo_l476rg apps-public/nucleoL476_USART3Echo
```

### Flash to the board
```bash
west flash
```

## Usage

### Shell Interface (Console)
1. Connect the board via USB
2. Open a serial terminal to the console port (115200 baud)
3. Use the shell commands to control the LED

Example:
```
nucleo:~$ led status
LED Mode: FLASHING
Flash Period: 1000 ms
nucleo:~$ led on
LED set to ON
nucleo:~$ led off
LED set to OFF
```

### USART3 Echo
1. Connect a serial terminal or USB-to-serial adapter to:
   - PC3 (USART3_RX)
   - PC4 (USART3_TX)
   - GND
2. Configure terminal for 115200 baud, 8N1
3. Type characters - they will be echoed back
4. Welcome message displayed on connection:
   ```
   === USART3 Echo Ready ===
   Type characters to echo them back...
   ```

## Project Structure

```
nucleoL476_USART3Echo/
├── CMakeLists.txt              # Build configuration
├── prj.conf                    # Project configuration
├── boards/
│   └── nucleo_l476rg.overlay   # Board-specific USART3 pin configuration
├── src/
│   ├── main.c                  # Main application entry point
│   ├── main.h                  # Version definitions
│   ├── gpioHandler.c           # LED control implementation
│   ├── gpioHandler.h           # LED control API
│   ├── shellCommands.c         # Shell command implementations
│   ├── shellCommands.h         # Shell command API
│   ├── dvkUart.c               # UART echo implementation
│   └── dvkUart.h               # UART API
└── docs/
    └── README.md               # This file
```

## Code Architecture

### Tasks
The application uses two separate tasks:
1. **LED Control Task** (priority 7, 1KB stack): Handles LED state machine
2. **UART Echo Task** (priority 7, 2KB stack): Handles USART3 echo functionality

### UART Module (dvkUart)
- **Interrupt-driven reception**: Efficient character reception without polling
- **Ring buffer**: 256-byte circular buffer for received data
- **Semaphore-based signaling**: Task wakes on data reception
- **Echo functionality**: Characters are echoed back immediately
- **Extensible design**: Ready to add command parsing for future expansion

### Future Expansion
The UART module is designed to be extended with command parsing:
```c
// In uartEchoTask(), you can accumulate characters into a command buffer
// and parse commands on '\r' or '\n', then execute and respond
```

## Configuration

### UART Configuration (prj.conf)
```
CONFIG_UART_INTERRUPT_DRIVEN=y
```

### Pin Configuration (boards/nucleo_l476rg.overlay)
```
&usart3 {
    pinctrl-0 = <&usart3_tx_pc4 &usart3_rx_pc3>;
    pinctrl-names = "default";
    current-speed = <115200>;
    status = "okay";
};
```

## Version
1.0.0.0

## License
SPDX-License-Identifier: Apache-2.0

## Copyright
Copyright (c) 2025
