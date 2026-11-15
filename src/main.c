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

#ifndef DEVELOPMENT_SPI_KEYBOARD_INPUT
#define DEVELOPMENT_SPI_KEYBOARD_INPUT 1
#endif

#define SPI_INPUT_TIMEOUT_TICKS 6u

#define PLAYER_START_X	100.0f
#define PLAYER_FLOOR_Y	192.0f
#define PLAYER_MIN_X	0.0f
#define PLAYER_MAX_X	220.0f
#define PLAYER_WIDTH	16
#define PLAYER_HEIGHT	16

#define PLAYER_MOVE_SPEED			60.0f
#define PLAYER_GRAVITY				900.0f
#define PLAYER_JUMP_SPEED			300.0f
#define PLAYER_GROUND_DECELERATION 	300.0f


uint32_t previous_ms;
uint32_t game_accumulator;

void main(void){
	GameSnapshot snapshot = {
		.sequence = 0u,
		.player_x = 100,
		.player_y = 192,
		.camera_x = 0,
		.camera_y = 0,
		.player_state = PLAYER_IDLE,
		.facing_direction = FACING_RIGHT
	};
	object player_object;
	physics player_physics;
	player_stats player_statistics;
	player player_entity;
	input controls;
	uint8_t pi_packet[PROTOCOL_PACKET_SIZE];
	uint8_t buttons = 0u;

	#if DEVELOPMENT_SPI_KEYBOARD_INPUT
	uint32_t last_spi_input_generation = 0u;
	uint32_t spi_input_stale_ticks = SPI_INPUT_TIMEOUT_TICKS;
	#endif

	system_clock_init();
	tick_init();

	#if !DEVELOPMENT_SPI_KEYBOARD_INPUT
	input_controller_init(16000000u);
	#endif

	object_init(
		&player_object,
		PLAYER_START_X,
		PLAYER_FLOOR_Y,
		PLAYER_WIDTH,
		PLAYER_HEIGHT,
		OBJECT_TYPE_PLAYER
	);
	physics_init(
		&player_physics,
		0.0f,
		0.0f,
		0.0f,
		PLAYER_GRAVITY,
		0.0f
	);
	player_init(
		&player_entity,
		&player_object,
		&player_physics,
		&player_statistics
	);
	player_resolve_floor(&player_entity, PLAYER_FLOOR_Y);

	#if DEVELOPMENT_SPI_KEYBOARD_INPUT
	input_init(&controls, KEYBOARD_INPUT);
	#else
	input_init(&controls, CONTROLLER_INPUT);
	#endif

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

			#if DEVELOPMENT_SPI_KEYBOARD_INPUT
			{
			    uint32_t input_generation =
			        spi_slave_input_generation();
			
			    if (input_generation != last_spi_input_generation) {
			        buttons = spi_slave_input_read();
			        last_spi_input_generation = input_generation;
			        spi_input_stale_ticks = 0u;
			    }
			    else if (spi_input_stale_ticks < SPI_INPUT_TIMEOUT_TICKS) {
			        ++spi_input_stale_ticks;
			    }
			
			    if (spi_input_stale_ticks >= SPI_INPUT_TIMEOUT_TICKS) {
			        buttons = 0u;
			    }
			}
			#else
			buttons = input_controller_read();
			#endif			

			// update player movement
			input_update(&controls, buttons);

			if(input_down(&controls, INPUT_BUTTON_LEFT) != 0){
				physics_set_velocity_x(&player_physics, -PLAYER_MOVE_SPEED);
				snapshot.facing_direction = FACING_LEFT;
			}
			else if(input_down(&controls, INPUT_BUTTON_RIGHT) != 0u){
				physics_set_velocity_x(&player_physics, PLAYER_MOVE_SPEED);
				snapshot.facing_direction = FACING_RIGHT;
			}
			else if(
			    (buttons & (INPUT_BUTTON_LEFT | INPUT_BUTTON_RIGHT)) ==
			    (INPUT_BUTTON_LEFT | INPUT_BUTTON_RIGHT)
			){
			    physics_set_velocity_x(&player_physics, 0.0f);
			}
			else if(
			    player_is_grounded(&player_entity) &&
			    (input_pressed(&controls, INPUT_BUTTON_A) == 0u)
			){
			    player_apply_ground_deceleration(
			        &player_entity,
			        PLAYER_GROUND_DECELERATION,
			        FIXED_DELTA_TIME
			    );
			}

			if(input_pressed(&controls, INPUT_BUTTON_A) != 0u){
				(void)player_try_jump(&player_entity, PLAYER_JUMP_SPEED);
			}

			player_update(&player_entity, FIXED_DELTA_TIME);

			player_resolve_floor(&player_entity, PLAYER_FLOOR_Y);

			if(player_object.x < PLAYER_MIN_X){
				player_object.x = PLAYER_MIN_X;
				if(player_physics.velocity_x < 0.0f){
					player_physics.velocity_x = 0.0f;
				}
			}
			else if(player_object.x > PLAYER_MAX_X){
				player_object.x = PLAYER_MAX_X;
				if(player_physics.velocity_x > 0.0f){
					player_physics.velocity_x = 0.0f;
				}
			}

			snapshot.player_x = (int16_t)player_object.x;
			snapshot.player_y = (int16_t)player_object.y;

			if(!player_is_grounded(&player_entity)){
				snapshot.player_state = PLAYER_JUMPING;
			}
			else if(player_physics.velocity_x != 0.0f){
				snapshot.player_state = PLAYER_WALKING;
			}
			else{
				snapshot.player_state = PLAYER_IDLE;
			}



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
