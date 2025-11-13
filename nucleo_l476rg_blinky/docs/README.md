# Zephyr Public Applications

This repository contains public Zephyr RTOS applications.

## Structure

```
apps-public/
├── nucleo_l476rg_blinky/    # LED blinky for Nucleo-L476RG
└── ...                       # Other public applications
```

## Prerequisites

- Zephyr SDK installed
- West tool installed
- Zephyr workspace set up in `../zephyrproject/`

## Building Applications

### Option 1: Build from Zephyr Workspace (Recommended)

```bash
# From the zephyrproject directory
cd zephyrproject
west build -b <board_name> ../apps-public/<app_name>
```

Example:
```bash
cd zephyrproject
west build -b nucleo_l476rg ../apps-public/nucleo_l476rg_blinky
```

### Option 2: Build with Direct Commands

```bash
# Navigate to zephyrproject
cd zephyrproject

# Build specific app
west build -b nucleo_l476rg ../apps-public/nucleo_l476rg_blinky

# Clean build
west build -b nucleo_l476rg -p always ../apps-public/nucleo_l476rg_blinky
```

## Flashing

After building, flash to your board:

```bash
# From zephyrproject directory
west flash
```

## Applications

### nucleo_l476rg_blinky
Simple LED blinky for STM32 Nucleo-L476RG board.

## Contributing

This is a public repository. Contributions are welcome!

## License

Apache-2.0
