#include "input.h"

typedef enum input_type{
    CONTROLLER_INPUT,
    KEYBOARD_INPUT
} input_type;

typedef struct input{
    input_type type;
    uint8_t buttons;
    uint8_t previous_buttons;
} input;

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



#ifdef INPUT_TM4C1294XL
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#define CONTROLLER_CLK GPIO_PIN_0
#define CONTROLLER_LATCH GPIO_PIN_1
#define CONTROLLER_DATA GPIO_PIN_2

static uint32_t controller_delay;

void input_controller_init(uint32_t system_clock_hz){
    /* SysCtlDelay uses three cycles per iteration. Round up to a 5 us half
     * period for a clock no faster than 100 kHz, excluding GPIO overhead. */
    controller_delay = system_clock_hz / 600000u;
    if (system_clock_hz % 600000u != 0 || controller_delay == 0){
        ++controller_delay;
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ)){
    }

    /* Preload output levels before enabling the output drivers. */
    GPIOPinWrite(GPIO_PORTQ_BASE, CONTROLLER_CLK | CONTROLLER_LATCH,
                 CONTROLLER_LATCH);
    GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, CONTROLLER_CLK | CONTROLLER_LATCH);
    GPIOPinTypeGPIOInput(GPIO_PORTQ_BASE, CONTROLLER_DATA);
}

uint8_t input_controller_read(void){
    uint8_t buttons = 0;
    uint32_t bit;

    GPIOPinWrite(GPIO_PORTQ_BASE, CONTROLLER_CLK | CONTROLLER_LATCH, 0);
    SysCtlDelay(controller_delay);
    GPIOPinWrite(GPIO_PORTQ_BASE, CONTROLLER_LATCH, CONTROLLER_LATCH);
    SysCtlDelay(controller_delay);

    for (bit = 1; bit <= 0x80u; bit <<= 1){
        /* H (the A button) is already at QH after loading: sample BEFORE
         * the rising clock edge. Pull-ups make a pressed button read low. */
        if (GPIOPinRead(GPIO_PORTQ_BASE, CONTROLLER_DATA) == 0){
            buttons |= (uint8_t)bit;
        }
        GPIOPinWrite(GPIO_PORTQ_BASE, CONTROLLER_CLK, CONTROLLER_CLK);
        SysCtlDelay(controller_delay);
        GPIOPinWrite(GPIO_PORTQ_BASE, CONTROLLER_CLK, 0);
        SysCtlDelay(controller_delay);
    }

    return buttons;
}
#endif
