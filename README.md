# Public Zephyr Applications

This repository contains publicly shareable Zephyr RTOS applications.

## Applications

- **nucleo_l476rg_blinky**: Basic LED blink example for STM32 Nucleo-L476RG board

## Prerequisites

This repository is designed to work with a parent workspace that contains:
- Zephyr RTOS installation in `../zephyrproject/`
- Python virtual environment activation scripts in `../`

## Getting Started

1. Clone this repository into your workspace alongside your Zephyr installation
2. Open this folder as a workspace in VSCode
3. Use the VSCode tasks (Ctrl+Shift+P > "Tasks: Run Task") to build and flash applications

## Build Tasks

The `.vscode/tasks.json` file provides the following tasks:
- **Build**: Compile the application
- **Clean Build**: Rebuild with incremental cleaning
- **Pristine Build**: Full clean rebuild
- **Flash**: Flash the built application to the target
- **Build and Flash**: Combined build and flash operation
- **Build Size Report**: View ROM/Flash usage
- **RAM Report**: View RAM usage
- **menuconfig**: Configure application settings

## Adding New Applications

1. Create a new folder for your application
2. Add required files: `CMakeLists.txt`, `prj.conf`, `src/main.c`
3. Add corresponding build tasks to `.vscode/tasks.json`

## License

Add your license information here.
