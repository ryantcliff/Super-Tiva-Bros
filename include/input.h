#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

/* TES-CTRL-MK1 serial button order, sampled from Q7 first. */
#define INPUT_BUTTON_A      (1u << 0)
#define INPUT_BUTTON_B      (1u << 1)
#define INPUT_BUTTON_SELECT (1u << 2)
#define INPUT_BUTTON_START  (1u << 3)
#define INPUT_BUTTON_UP     (1u << 4)
#define INPUT_BUTTON_DOWN   (1u << 5)
#define INPUT_BUTTON_LEFT   (1u << 6)
#define INPUT_BUTTON_RIGHT  (1u << 7)

typedef enum input_type {
    CONTROLLER_INPUT,
    KEYBOARD_INPUT
} input_type;

typedef struct input {
    input_type type;
    uint8_t buttons;
    uint8_t previous_buttons;
} input;

void input_init(input *state, input_type type);
void input_update(input *state, uint32_t buttons);
uint32_t input_down(const input *state, uint32_t buttons);
uint32_t input_pressed(const input *state, uint32_t buttons);
uint32_t input_released(const input *state, uint32_t buttons);
void input_controller_init(uint32_t system_clock_hz);
uint8_t input_controller_read(void);

#endif
