#include "audio.h"
#include "block.h"
#include "camera.h"
#include "collision.h"
#include "display.h"
#include "enemy.h"
#include "game.h"
#include "input.h"
#include "interactable.h"
#include "item.h"
#include "level.h"
#include "object.h"
#include "physics.h"
#include "player.h"
#include "projectile.h"
#include "render.h"
#include "spi.h"
#include "statistics.h"
#include "timing.h"
#include "uart.h"

static char hex_digit(uint8_t value){
    value &= 0x0Fu;
    if (value < 10u){
        return (char)('0' + (int)value);
    }
    return (char)('A' + (int)value - 10);
}

static void print_controller_state(uint8_t buttons){
    uart_write_string("Controller: 0x");
    uart_write_char(hex_digit(buttons >> 4));
    uart_write_char(hex_digit(buttons));
    uart_write_string("\r\n");
}

int main(void){
    uint8_t buttons;
    uint8_t previous_buttons;

    uart_init();
    uart_write_string("Super Tiva Bros ready\r\n");
    input_controller_init(16000000u);

    buttons = input_controller_read();
    previous_buttons = buttons;
    print_controller_state(buttons);
    spi_slave_init(buttons);

    for (;;){
        buttons = input_controller_read();

        if (buttons != previous_buttons){
            print_controller_state(buttons);
            previous_buttons = buttons;
        }

        (void)spi_slave_service(buttons);
    }
}
