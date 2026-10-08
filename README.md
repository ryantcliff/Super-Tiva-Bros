# Super Tiva Bros

Super Tiva Bros is an embedded systems project that brings a classic
side-scrolling platform game to custom hardware. An EK-TM4C1294XL LaunchPad
runs the game simulation and reads a custom NES-style controller, while a
Raspberry Pi 2 renders the scene for an RCA CRT television.

The project is being developed by **Ryan Antcliff** and **Jesse Maldonado** as
part of CSE 479. It combines embedded C, digital hardware, serial communication,
real-time game logic, PCB design, and retro video output in one system.

> **Project status:** Active development. The CCS project builds and launches
> in the debugger, and the core object, physics, collision, camera, timing, and
> entity-update modules are in place. Controller hardware integration, the SPI
> protocol, Raspberry Pi renderer, and CRT output are still in progress.

## System architecture

```text
Custom NES-style controller
          │
          │ SN74HC165 serial button data
          ▼
EK-TM4C1294XL LaunchPad
  • game state and rules
  • movement and physics
  • collision detection
  • controller input
          │
          │ compact game state over SPI (planned)
          ▼
Raspberry Pi 2 Model B V1.1
  • sprites and tile assets
  • scene composition
  • composite video output
          │
          ▼
RCA XL-100 GER685LR CRT television
```

The Tiva will send state such as entity positions, animation states, camera
offsets, score, and tile changes. The Pi will construct each video frame rather
than receiving a full framebuffer from the microcontroller. This keeps the Tiva
focused on deterministic game behavior and assigns rendering to hardware better
suited to graphics and video output.

## Hardware

| Component | Role |
| --- | --- |
| EK-TM4C1294XL | Main game controller using the TM4C1294NCPDT Cortex-M4F MCU |
| Raspberry Pi 2 Model B V1.1 | Graphics processor and composite-video source |
| SN74HC165N | Parallel-in/serial-out register for the eight controller buttons |
| Custom NES-style controller | Player input using A, B, Start, Select, and a D-pad |
| RCA XL-100 GER685LR | Final CRT display |
| External RF modulator | Required if the television is used through its coaxial RF input |

Both processors use 3.3 V GPIO logic. The final SPI connection will include a
shared ground and verified pin assignments before the boards are connected.
Only the television's external inputs will be used.

## Controller interface

The controller design uses an SN74HC165N to capture eight active-low button
signals and shift them to the Tiva over three GPIO lines:

| TM4C1294XL pin | Controller signal |
| --- | --- |
| PQ0 | Clock |
| PQ1 | Parallel load (`/SH/LD`) |
| PQ2 | Serial data (`QH`) |

The intended controller byte follows the traditional NES ordering:

| Bit | Button |
| ---: | --- |
| 0 | A |
| 1 | B |
| 2 | Select |
| 3 | Start |
| 4 | Up |
| 5 | Down |
| 6 | Left |
| 7 | Right |

The optional hardware reader in `src/input.c` is enabled with the
`INPUT_TM4C1294XL` preprocessor symbol. Hardware debounce has not yet been
implemented.

## Current implementation

The repository currently includes:

- Reusable object and entity models for players, enemies, blocks, items,
  projectiles, interactables, and levels
- Semi-implicit Euler integration for velocity, acceleration, and position
- Axis-aligned bounding-box collision checks
- Camera following, visibility checks, and world/screen coordinate conversion
- Frame timing state with rollover-safe millisecond updates
- Central game-state ownership and per-frame entity updates
- Initial SN74HC165 controller-reading code for the EK-TM4C1294XL
- CCS Debug and Release configurations for the TM4C1294NCPDT
- Startup code, interrupt vector table, linker script, and ICDI target
  configuration

The application entry point is currently an idle scaffold. Audio, rendering,
level content, the complete game loop, and communication with the Pi remain to
be implemented.

## Development environment

- Code Composer Studio 12.8.1
- TI ARM Compiler 20.2.7.LTS
- TivaWare for C Series 2.2.0.295
- C99
- Raspberry Pi OS compatible with Raspberry Pi 2 Model B V1.1 (planned)
- KiCad 10 for controller PCB development

The checked-in CCS configuration currently references TivaWare at
`C:/ti/TivaWare_C_Series-2.2.0.295`. Developers using another installation
location must update the compiler include path and driver library path in both
the Debug and Release configurations.

## Build and debug

1. Clone the repository.
2. Open Code Composer Studio.
3. Select **Project > Import CCS Projects**.
4. Choose the repository root and import **Super-Tiva-Bros**.
5. Leave **Copy projects into workspace** unchecked so CCS and Git use the same
   files.
6. Connect the EK-TM4C1294XL through its onboard ICDI debug interface.
7. Select the **Debug** configuration, build the project, and start a debug
   session.

Generated Debug and Release output is excluded from version control. The CCS
project metadata, linker script, startup code, and target configuration are
tracked so both project partners can import the same project settings.

## Repository layout

```text
Super-Tiva-Bros/
├── include/                         Public C interfaces and data types
├── src/                             Game and hardware implementation
├── targetConfigs/                   CCS target/debug configuration
├── tests/                           Host-side test area
├── tm4c1294ncpdt_startup_ccs.c      Startup code and interrupt vectors
├── tm4c1294ncpdt.cmd                TM4C1294 linker memory map
├── .ccsproject / .cproject          Shared CCS project configuration
└── README.md
```

## Roadmap

- Complete the TM4C1294 system clock, timing, UART, and controller bring-up
- Validate all controller buttons and simultaneous button presses on hardware
- Build the first playable level and complete movement/collision behavior
- Define and test a versioned Tiva-to-Pi SPI packet format
- Develop the Raspberry Pi tile and sprite renderer over HDMI
- Move the renderer to composite output and validate it on a safe external input
- Connect the finished system to the CRT through the appropriate video path
- Integrate audio, polish gameplay, and document the completed hardware

## Project team

- **Ryan Antcliff**
- **Jesse Maldonado**

This is a shared project. Design, implementation, documentation, and repository
decisions should be reviewed with both project partners as development
continues.

## License

This repository is available under the terms in [LICENSE](LICENSE).

Super Tiva Bros is an educational project inspired by classic platform games.
It is not affiliated with or endorsed by Nintendo.
