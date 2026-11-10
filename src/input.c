#include "input.h"
#include "uart.h"

void input_init(input *state, input_type type){
    state->type = type;
    state->buttons = 0;
    state->previous_buttons = 0;
}

void input_update(input *state, uint32_t buttons){
    const uint32_t horizontal = INPUT_BUTTON_LEFT | INPUT_BUTTON_RIGHT;
    const uint32_t vertical = INPUT_BUTTON_UP | INPUT_BUTTON_DOWN;

    /* Opposing directions cancel; independent buttons remain available. */
    if ((buttons & horizontal) == horizontal){
        buttons &= ~horizontal;
    }
    if ((buttons & vertical) == vertical){
        buttons &= ~vertical;
    }
    state->previous_buttons = state->buttons;
    state->buttons = buttons;
}

uint32_t input_down(const input *state, uint32_t buttons){
    return state->buttons & buttons;
}

uint32_t input_pressed(const input *state, uint32_t buttons){
    return state->buttons & ~state->previous_buttons & buttons;
}

uint32_t input_released(const input *state, uint32_t buttons){
    return state->previous_buttons & ~state->buttons & buttons;
}

#define HWREG(address) (*((volatile uint32_t *)(address)))

#define GPIO_PORTE_BASE 0x4005C000u
#define GPIO_PORTE_DATA(mask) \
    HWREG(GPIO_PORTE_BASE + ((uint32_t)(mask) << 2))

#define CONTROLLER_CLK   0x01u
#define CONTROLLER_LATCH 0x02u
#define CONTROLLER_DATA  0x04u

static uint32_t controller_delay;

extern void input_controller_gpio_init(void);
extern void input_controller_delay(uint32_t iterations);

void input_controller_init(uint32_t system_clock_hz){
    /* The assembly delay uses three cycles per iteration. Round up to a 5 us half
     * period for a clock no faster than 100 kHz, excluding GPIO overhead. */
    controller_delay = system_clock_hz / 600000u;
    if (system_clock_hz % 600000u != 0 || controller_delay == 0){
        ++controller_delay;
    }

    input_controller_gpio_init();
}

uint8_t input_controller_read(void){
    uint8_t buttons = 0;
    uint32_t bit;

    GPIO_PORTE_DATA(CONTROLLER_CLK | CONTROLLER_LATCH) = 0u;
    input_controller_delay(controller_delay);
    GPIO_PORTE_DATA(CONTROLLER_LATCH) = CONTROLLER_LATCH;
    input_controller_delay(controller_delay);

    for (bit = 1; bit <= 0x80u; bit <<= 1){
        /* H (the A button) is already at QH after loading: sample BEFORE
         * the rising clock edge. Pull-ups make a pressed button read low. */
        if (GPIO_PORTE_DATA(CONTROLLER_DATA) == 0u){
            buttons |= (uint8_t)bit;
        }
        GPIO_PORTE_DATA(CONTROLLER_CLK) = CONTROLLER_CLK;
        input_controller_delay(controller_delay);
        GPIO_PORTE_DATA(CONTROLLER_CLK) = 0u;
        input_controller_delay(controller_delay);
    }

    return buttons;
}

