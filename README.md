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
> entity-update modules are in place. The Tiva-to-Pi one-byte SPI input path and
> an initial Pygame sprite renderer are working. The physical controller, full
> game-state protocol, playable levels, and CRT output remain in progress.

## System architecture

```text
Custom NES-style controller (TES-CTRL-MK1)
          │
          │ AiP74HC165 serial button data
          ▼
EK-TM4C1294XL LaunchPad
  • game state and rules
  • movement and physics
  • collision detection
  • controller input
          │
          │ controller byte over SPI (verified)
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
| AiP74HC165N | Parallel-in/serial-out register for the eight controller buttons |
| TES-CTRL-MK1 | Player input using A, B, Start, Select, and a D-pad |
| RCA XL-100 GER685LR | Final CRT display |
| External RF modulator | Required if the television is used through its coaxial RF input |

Both processors use 3.3 V GPIO logic. The current SPI connection uses a shared
ground and the verified pin assignments documented below; the Pi and Tiva power
rails are not tied together. Only the television's external inputs will be
used.

## Controller interface

The controller design uses an AiP74HC165N to capture eight active-low button
signals and shift them to the Tiva over three GPIO lines:

| TM4C1294XL pin | Controller signal |
| --- | --- |
| PE0 (X9-5) | Clock (`CP`) |
| PE1 (X9-7) | Parallel load (`/PL`) |
| PE2 (X9-9) | Serial data (`Q7`) |

The intended controller byte uses the following ordering:

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

The hardware reader in `src/input.c` targets the TM4C1294XL directly. Hardware
debounce has not yet been implemented.

### Tiva-to-Pi SPI input

The Raspberry Pi is the SPI master and polls the TM4C1294XL SSI2 slave at
100 kHz using SPI mode 0 and 8-bit transfers. The Tiva preloads its latest
active-high controller byte so the Pi can read it through `/dev/spidev0.0`.

| Raspberry Pi 2 | EK-TM4C1294XL |
| --- | --- |
| Pin 23 / GPIO11 / SCLK | PD3 / SSI2CLK |
| Pin 24 / GPIO8 / CE0 | PD2 / SSI2FSS |
| Pin 21 / GPIO9 / MISO | PD1 / SSI2TX |
| Pin 19 / GPIO10 / MOSI | PD0 / SSI2RX |
| Pin 6 or 25 / GND | GND |

Do not connect the Pi's 5 V pin to the Tiva. Power down both boards before
changing the wiring.

Enable SPI on Raspberry Pi OS through `raspi-config`, then verify that
`/dev/spidev0.0` exists and Python can import `spidev`:

```bash
ls -l /dev/spidev0.0
python3 -c 'import spidev; print("spidev ready")'
```

Copy the current Pi input and graphics files from PowerShell:

```powershell
scp .\pi\controller_protocol.py .\pi\ssh_keyboard_input.py .\pi\tiva_spi_input.py .\pi\spi_controller_test.py .\pi\pygame_move_test.py antonado@stb:~/
scp -r .\pi\assets antonado@stb:~/assets
```

Monitor controller bytes without starting the renderer:

```bash
python3 ~/spi_controller_test.py
```

The link has been verified first with a fixed `0x80` response and then with a
live `0x00` controller state matching the Tiva's UART output. The physical
TES-CTRL-MK1 is still under hardware debugging, so other live button values are
not yet considered validated.

### Temporary SSH keyboard input

The physical controller remains under hardware debugging. Development can
continue with the Windows laptop keyboard through an interactive SSH session on
the Raspberry Pi. The dependency-free `pi/ssh_keyboard_input.py` program emits
the same active-high 8-bit state planned for the controller. This input path has
been verified on the Raspberry Pi 2, including all eight buttons and release
transitions back to `0x00`.

From PowerShell in the repository root, copy the keyboard module and shared
protocol definitions to the Pi:

```powershell
scp .\pi\controller_protocol.py .\pi\ssh_keyboard_input.py antonado@stb:~/
```

Connect over SSH and launch it:

```powershell
ssh antonado@stb
```

```bash
python3 ~/ssh_keyboard_input.py
```

| Laptop key | Button | Mask |
| --- | --- | ---: |
| Z | A | `0x01` |
| X | B | `0x02` |
| Space | Select | `0x04` |
| Enter | Start | `0x08` |
| Up arrow | Up | `0x10` |
| Down arrow | Down | `0x20` |
| Left arrow | Left | `0x40` |
| Right arrow | Right | `0x80` |

Press Q or Ctrl+C to exit. SSH terminals do not provide true key-release
events, so each key remains active for 200 ms and keyboard-repeat events extend
that interval. This is suitable for early gameplay and renderer development,
but simultaneous inputs are less precise than the finished controller.

### Pygame graphics test

`pi/pygame_move_test.py` displays the Tiva character sprite at a logical
320x240 resolution and can use either input transport without source changes.
On the Pi desktop, launch it with the Tiva SPI input:

```bash
DISPLAY=:0 SDL_VIDEODRIVER=x11 python3 ~/pygame_move_test.py --input spi
```

Use the temporary SSH keyboard instead with:

```bash
DISPLAY=:0 SDL_VIDEODRIVER=x11 python3 ~/pygame_move_test.py --input keyboard
```

Both launch modes have been verified on the Raspberry Pi 2. The SPI mode
currently remains stationary when the Tiva reports the neutral `0x00` state.

## Current implementation

The repository currently includes:

- Reusable object and entity models for players, enemies, blocks, items,
  projectiles, interactables, and levels
- Semi-implicit Euler integration for velocity, acceleration, and position
- Axis-aligned bounding-box collision checks
- Camera following, visibility checks, and world/screen coordinate conversion
- Frame timing state with rollover-safe millisecond updates
- Central game-state ownership and per-frame entity updates
- Initial AiP74HC165N controller-reading code for the EK-TM4C1294XL
- Direct-register SSI2 slave code publishing the controller byte to the Pi
- A tested `spidev` input adapter and terminal SPI controller monitor
- Verified SSH keyboard input on the Raspberry Pi using the controller protocol
- A Pygame graphics test with Tiva sprite assets and selectable SPI/keyboard input
- CCS Debug and Release configurations for the TM4C1294NCPDT
- Startup code, interrupt vector table, linker script, and ICDI target
  configuration

The application entry point reads the controller, reports button-mask changes
over UART, and publishes the latest byte through SSI2. Audio, level content, the
complete game loop, the full Tiva-to-Pi game-state protocol, and CRT output
remain to be implemented.

## Development environment

- Code Composer Studio 12.8.1
- TI ARM Compiler 20.2.7.LTS
- TivaWare for C Series 2.2.0.295
- C99
- Raspberry Pi OS Legacy (32-bit, Debian Bookworm) with Desktop
- Python 3, Pygame, and `spidev`
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
├── pi/                              Pi input, graphics tests, and sprite assets
├── targetConfigs/                   CCS target/debug configuration
├── tests/                           Host-side Python tests
├── tm4c1294ncpdt_startup_ccs.c      Startup code and interrupt vectors
├── tm4c1294ncpdt.cmd                TM4C1294 linker memory map
├── .ccsproject / .cproject          Shared CCS project configuration
└── README.md
```

## Roadmap

- Complete the TM4C1294 system clock and controller bring-up
- Validate all controller buttons and simultaneous button presses on hardware
- Build the first playable level and complete movement/collision behavior
- Extend the verified controller byte into a versioned game-state SPI protocol
- Extend the Raspberry Pi sprite test into a tile and scene renderer over HDMI
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
