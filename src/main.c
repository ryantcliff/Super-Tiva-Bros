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
#include "tick.h"
#include "protocol.h"
#include "system.h"

#define GAME_UPDATE_HZ		60u
#define FIXED_DELTA_TIME	(1.0f/60.0f)

uint32_t previous_ms;
uint32_t game_accumulator;

void main(void){
	GameSnapshot snapshot = {
		.sequence = 0u,
		.player_x = 100,
		.player_y = 150,
		.camera_x = 0,
		.camera_y = 0,
		.player_state = PLAYER_IDLE,
		.facing_direction = FACING_LEFT
	};
	uint8_t pi_packet[PROTOCOL_PACKET_SIZE];

	system_clock_init();
	tick_init();

	if(protocol_encode(&snapshot, pi_packet) != PROTOCOL_PACKET_SIZE){
		while(1){}
	}
	spi_slave_init(pi_packet);

	previous_ms = tick_millis();
	game_accumulator = 0u;

	for(;;){
		uint32_t current_ms = tick_millis();
		uint32_t elapsed_time = current_ms - previous_ms;
		previous_ms = current_ms;

		game_accumulator += elapsed_time * GAME_UPDATE_HZ;

		while(game_accumulator >= 1000u){

			// read controller

			// update player movement

			// update physics / game

			// update camera
		
			++snapshot.sequence;
			if(protocol_encode(&snapshot, pi_packet) == PROTOCOL_PACKET_SIZE){
				(void)spi_slave_publish(pi_packet);
			}

			game_accumulator -= 1000u;
		}
	}

}
