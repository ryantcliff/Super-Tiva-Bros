#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

typedef enum {
    INPUT_TYPE_KEYBOARD,
    INPUT_TYPE_GAMEPAD
} input_type;

typedef enum {
    INPUT_BUTTON_A      = 1u << 0,
    INPUT_BUTTON_B      = 1u << 1,
    INPUT_BUTTON_SELECT = 1u << 2,
    INPUT_BUTTON_START  = 1u << 3,
    INPUT_BUTTON_UP     = 1u << 4,
    INPUT_BUTTON_DOWN   = 1u << 5,
    INPUT_BUTTON_LEFT   = 1u << 6,
    INPUT_BUTTON_RIGHT  = 1u << 7,
    INPUT_BUTTON_JUMP   = INPUT_BUTTON_A
} input_button;

typedef struct input {
    input_type type;
    uint32_t buttons;
    uint32_t previous_buttons;
} input;

/*  State must be non-null. The platform maps device buttons to INPUT_BUTTON_*
    and calls input_update once per frame, including frames with no buttons down.
    Queries return the matching bits; edges remain available until the next update. */
void input_init(input *state, input_type type);
void input_update(input *state, uint32_t buttons);
uint32_t input_down(const input *state, uint32_t buttons);
uint32_t input_pressed(const input *state, uint32_t buttons);
uint32_t input_released(const input *state, uint32_t buttons);

#ifdef INPUT_TM4C1294XL
/*  Requires TivaWare driverlib. Call after configuring the system clock, using
    its actual frequency in Hz, and again if that frequency changes. Owns PQ0-2.
    Read from one execution context after initialization; returns raw pressed
    bits (no debounce). Pass the result to input_update once per game frame. */
void input_controller_init(uint32_t system_clock_hz);
uint8_t input_controller_read(void);
#endif

#endif
