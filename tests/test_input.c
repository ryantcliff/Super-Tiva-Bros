#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "input.h"

void input_controller_gpio_init(void){}

void input_controller_delay(uint32_t iterations){
    (void)iterations;
}

int main(void){
    input controls;
    input_init(&controls, KEYBOARD_INPUT);
    input_update(
        &controls,
        INPUT_BUTTON_LEFT | INPUT_BUTTON_RIGHT
    );
    assert(
        input_down(
            &controls,
            INPUT_BUTTON_LEFT | INPUT_BUTTON_RIGHT
        ) == 0u
    );
    input_update(&controls, INPUT_BUTTON_A);
    assert(input_pressed(&controls, INPUT_BUTTON_A) != 0u);
    assert(input_down(&controls, INPUT_BUTTON_A) != 0u);
    input_update(&controls, INPUT_BUTTON_A);
    assert(input_pressed(&controls, INPUT_BUTTON_A) == 0u);
    input_update(&controls, 0u);
    assert(input_released(&controls, INPUT_BUTTON_A) != 0u);
    input_update(&controls, INPUT_BUTTON_A);
    assert(input_pressed(&controls, INPUT_BUTTON_A) != 0u);
    puts("input state test passed");
    return 0;
}