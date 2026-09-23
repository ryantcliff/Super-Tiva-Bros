# Super-Tiva-Bros
Super Tiva Bros for the TM4C1294XL (CSE 479).

The NES-style controller uses an SN74HC165 at 3.3 V, with PQ0 connected to
CLK, PQ1 to /SH/LD, and PQ2 to QH. Wire register inputs H through A to the
A, B, Select, Start, Up, Down, Left, and Right buttons, respectively. Buttons
are active low; the input driver returns one bits for pressed buttons.

For the CCS hardware build, define `INPUT_TM4C1294XL`, add the TivaWare root
to the compiler include path, and link its matching `driverlib` library.
After configuring the system clock, initialize and poll the controller:

```c
input controls;
input_init(&controls, INPUT_TYPE_GAMEPAD);
input_controller_init(system_clock_hz); /* Actual configured CPU frequency. */

/* Once per game frame: */
input_update(&controls, input_controller_read());
if (input_pressed(&controls, INPUT_BUTTON_JUMP)) {
    /* Begin a jump. */
}
```

Include `input.h` in the caller. A maps to Jump and B to Run. Opposing D-pad
directions cancel in `input_update`. Reads are raw; switch debounce is not yet
implemented. Host builds can omit `INPUT_TM4C1294XL` and supply button masks
directly to `input_update`.
