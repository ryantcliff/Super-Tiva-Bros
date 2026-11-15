# Super Tiva Bros

Super Tiva Bros is an embedded systems project that brings a classic
side-scrolling platform game to custom hardware. An EK-TM4C1294XL LaunchPad
runs the game simulation and interfaces with a custom NES-style controller,
while a Raspberry Pi 2 renders the scene for an RCA CRT television.

The project is being developed by **Ryan Antcliff** and **Jesse Maldonado Ramos** as
part of CSE 479. It combines embedded C, digital hardware, serial communication,
real-time game logic, PCB design, and retro video output in one system.

> **Project status:** Active development. The TM4C1294XL now runs a 60 Hz game
> loop and publishes versioned 13-byte game snapshots through an
> interrupt-driven SSI2 slave. A Raspberry Pi 2 polls those snapshots over SPI
> and renders the player position, facing direction, and walking animation in
> Pygame. For development, SSH keyboard input travels back to the Tiva during
> the same full-duplex SPI transfer. The physical controller, gameplay systems,
> playable levels, composite-video output, and CRT integration remain in
> progress.

## System architecture

```text
Custom NES-style controller                SSH keyboard (temporary)
         │                                           │
         │ AiP74HC165 serial data                    │ controller mask
         ▼                                           ▼
EK-TM4C1294XL LaunchPad ◄──────── SPI MOSI ─── Raspberry Pi 2
  • authoritative game state                         • SPI master
  • 60 Hz movement and rules                         • snapshot decoder
  • physics and collision                            • Pygame renderer
         │                                           │
         └───────── SPI MISO: 13-byte snapshot ──────┘
                                                     │
                                                     ▼
                                       Composite video → RCA CRT
```

The Tiva sends compact state rather than a framebuffer. The current snapshot
contains sequence, player and camera coordinates, player state, and facing
direction. Future revisions will add visible entities and game events. The Pi
constructs each video frame, keeping deterministic gameplay on the Tiva while
assigning graphics and video output to the Pi.

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

### Bidirectional Tiva-to-Pi SPI link

The Raspberry Pi is the SPI master and polls the TM4C1294XL SSI2 slave at
100 kHz using SPI mode 0 and 8-bit transfers. Each transaction is 13 bytes and
uses `/dev/spidev0.0`.

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

The Tiva-to-Pi response has this little-endian layout:

| Byte(s) | Field |
| --- | --- |
| 0 | Magic (`0xA5`) |
| 1 | Protocol version (`0x01`) |
| 2 | Sequence number |
| 3–4 | Player X (`int16`) |
| 5–6 | Player Y (`int16`) |
| 7–8 | Camera X (`int16`) |
| 9–10 | Camera Y (`int16`) |
| 11 | Player state |
| 12 | Facing direction |

The SSI2 interrupt handler drains incoming bytes, fills the transmit FIFO, and
publishes complete snapshots only at packet boundaries. Two packet buffers keep
the Pi from observing a partially updated snapshot.

During temporary keyboard development, the Pi repeats one active-high
controller mask across all 13 MOSI bytes. Repetition avoids a second framing
scheme. The Tiva treats the input as neutral if no new SPI data arrives for six
game ticks (approximately 100 ms), preventing a disconnected Pi from leaving a
direction held.

Set `DEVELOPMENT_SPI_KEYBOARD_INPUT` in `src/main.c` to `1` for keyboard-over-SPI
or `0` for the physical TES-CTRL-MK1 path. Both configurations build in CCS.

### Temporary SSH keyboard input

The physical controller remains under hardware debugging. Development continues
with the Windows laptop keyboard through an interactive SSH session on the Pi.
`pi/ssh_keyboard_input.py` produces the same active-high controller byte planned
for the final hardware, and `pi/main.py` forwards it to the Tiva while receiving
the current game snapshot.

From PowerShell in the repository root, copy the Pi package to an existing clone:

```powershell
scp -r .\pi antonado@stb:~/Super-Tiva-Bros/
```

Connect over SSH and launch it:

```powershell
ssh antonado@stb
```

```bash
cd ~/Super-Tiva-Bros
DISPLAY=:0 SDL_VIDEODRIVER=x11 python3 -m pi.main
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

Press Q, Escape in the Pygame window, or Ctrl+C to exit. A clean shutdown sends
a final neutral SPI transfer and restores the SSH terminal. SSH terminals do
not provide true key-release events, so each key remains active for 200 ms and
keyboard-repeat extends that interval. This is suitable for development but is
not intended to replace the physical controller.

### Pygame renderer

`pi/main.py` is the current integrated renderer. It displays a 256x224 logical
scene, places the player at the Tiva-provided world coordinates, applies the
Tiva camera offset, and selects idle, walking, or jumping artwork from the
reported state. Walking frames alternate from the snapshot sequence number.
The live Raspberry Pi/Tiva test has verified horizontal movement, direction
changes, animation, and clean keyboard exit.

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
- Direct-register, interrupt-driven SSI2 slave code with TX/RX FIFO handling
- Double-buffered 13-byte snapshot streaming with packet-boundary publication
- A tested Pi decoder for versioned, signed, little-endian game snapshots
- Full-duplex temporary keyboard input from the Pi to the Tiva with a
  fail-neutral timeout
- A 256x224 Pygame renderer driven by Tiva-owned position and animation state
- Host-side Python and C tests for protocol encoding/decoding, packet streaming,
  SPI input handling, keyboard mapping, and sprite selection
- CCS Debug and Release configurations for the TM4C1294NCPDT
- Startup code, interrupt vector table, linker script, and ICDI target
  configuration

The current application entry point consumes the selected controller source,
updates horizontal movement at 60 Hz, encodes a game snapshot, and publishes it
to the Pi. Jumping, gravity, complete collision response, enemies, level
content, audio, and CRT output remain to be integrated into the live loop.

## Development environment

- Code Composer Studio 12.8.1
- TI ARM Compiler 20.2.7.LTS
- TivaWare for C Series 2.2.0.295
- C99
- Raspberry Pi OS (32-bit) with Desktop
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

Run the host-side Python tests from the repository root with:

```bash
python3 -m unittest discover -s tests -p "test_*.py" -v
```

Current host verification also includes strict C99 tests for protocol encoding,
packet streaming, and the SPI input mailbox. At the present milestone, all 28
Python tests and all three C test programs pass.

## Repository layout

```text
Super-Tiva-Bros/
├── include/                         Public C interfaces and data types
├── src/                             Game and hardware implementation
├── pi/                              Pi SPI transport, renderer, and sprite assets
├── targetConfigs/                   CCS target/debug configuration
├── tests/                           Host-side Python and C tests
├── tm4c1294ncpdt_startup_ccs.c      Startup code and interrupt vectors
├── tm4c1294ncpdt.cmd                TM4C1294 linker memory map
├── .ccsproject / .cproject          Shared CCS project configuration
└── README.md
```

## Roadmap

- Finish TES-CTRL-MK1 hardware debugging and replace the temporary SSH keyboard
  path
- Add jumping, gravity, and grounded-state handling to the Tiva game loop
- Integrate collision response and build the first playable level
- Extend snapshots with visible enemies, items, projectiles, score, and game
  events
- Expand the Pi renderer from the current player/background prototype into a
  tile and scene renderer
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
