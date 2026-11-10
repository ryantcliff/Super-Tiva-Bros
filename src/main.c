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

int main(void){
    uart_init();
    uart_write_string("Super Tiva Bros ready\r\n");
    spi_slave_init(INPUT_BUTTON_RIGHT);

    for (;;){
        (void)spi_slave_service(INPUT_BUTTON_RIGHT);
    }
}
