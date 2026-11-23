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
#include "level_data.h"

#define GAME_UPDATE_HZ		60u
#define FIXED_DELTA_TIME	(1.0f/60.0f)

#ifndef DEVELOPMENT_SPI_KEYBOARD_INPUT
#define DEVELOPMENT_SPI_KEYBOARD_INPUT 1
#endif

#define SPI_INPUT_TIMEOUT_TICKS 6u

#define PLAYER_MIN_X	0.0f
#define PLAYER_WIDTH	16
#define PLAYER_HEIGHT	16

#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT	224

#define PLAYER_MOVE_SPEED			60.0f
#define PLAYER_GRAVITY				900.0f
#define PLAYER_JUMP_SPEED			300.0f
#define PLAYER_GROUND_DECELERATION 	300.0f


uint32_t previous_ms;
uint32_t game_accumulator;

static bool game_load_level(
	LevelID level_id,
	const LevelDefinition **active_level,
	player *player_entity,
	camera *game_camera,
	GameSnapshot *snapshot
){
	const LevelDefinition *next = level_get(level_id);
	CollisionContacts contacts;

	if(next == NULL){
		return false;
	}

	*active_level = next;

	object_set_position(
		player_entity->object,
		next->spawn_x,
		next->spawn_y
	);

	physics_set_velocity(
		player_entity->physics,
		0.0f,
		0.0f
	);

	contacts = collision_move_static(
		player_entity->object,
		player_entity->physics,
		next->solids,
		next->solid_count,
		FIXED_DELTA_TIME
	);
	
	player_entity->grounded = contacts.grounded;

	camera_set_position(game_camera, 0.0f, 0.0f);

	snapshot->level_id = (uint8_t)next->id;
	snapshot->player_x = (int16_t)player_entity->object->x;
	snapshot->player_y = (int16_t)player_entity->object->y;
	snapshot->camera_x = 0;
	snapshot->camera_y = 0;
	snapshot->player_state = PLAYER_IDLE;
	snapshot->facing_direction = FACING_RIGHT;

	return true;
}

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
	camera game_camera;
	const LevelDefinition *active_level;
	CollisionContacts contacts;
	input controls;
	uint8_t pi_packet[PROTOCOL_PACKET_SIZE];
	uint8_t buttons = 0u;

	#if DEVELOPMENT_SPI_KEYBOARD_INPUT
	uint32_t last_spi_input_generation = 0u;
	uint32_t spi_input_stale_ticks = SPI_INPUT_TIMEOUT_TICKS;
	#endif

	system_clock_init();
	tick_init();

	active_level = level_get(LEVEL_ID_START);

	if(active_level == NULL){
		while(1){
			// level entered is invalid
			// enter infinite loop for safety
		}
	}

	snapshot.level_id = (uint8_t)active_level->id;

	#if !DEVELOPMENT_SPI_KEYBOARD_INPUT
	input_controller_init(16000000u);
	#endif

	object_init(
		&player_object,
		active_level->spawn_x,
		active_level->spawn_y,
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
	contacts = collision_move_static(
		&player_object,
		&player_physics,
		active_level->solids,
		active_level->solid_count,
		FIXED_DELTA_TIME
	);
	
	player_entity.grounded = contacts.grounded;

	camera_init(
		&game_camera,
		0.0f,
		0.0f,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);

	#if DEVELOPMENT_SPI_KEYBOARD_INPUT
	input_init(&controls, KEYBOARD_INPUT);
	#else
	input_init(&controls, CONTROLLER_INPUT);
	#endif

	snapshot.player_x = (int16_t)player_object.x;
	snapshot.player_y = (int16_t)player_object.y;

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

			contacts = collision_move_static(
				&player_object,
				&player_physics,
				active_level->solids,
				active_level->solid_count,
				FIXED_DELTA_TIME
			);
			player_entity.grounded = contacts.grounded;

			if(player_object.x < PLAYER_MIN_X){
				player_object.x = PLAYER_MIN_X;
				if(player_physics.velocity_x < 0.0f){
					player_physics.velocity_x = 0.0f;
				}
			}
			
			float max_player_x = (float)active_level->width - player_object.width;
			if(max_player_x < 0.0f){
				max_player_x = 0.0f;
			}
			if(player_object.x < 0.0f){
				player_object.x = 0.0f;
				if(player_physics.velocity_x < 0.0f){
					player_physics.velocity_x = 0.0f;
				}
			}
			else if(player_object.x > max_player_x){
				player_object.x = max_player_x;
				if(player_physics.velocity_x > 0.0f){
					player_physics.velocity_x = 0.0f;
				}
			}

			if(player_object.x >= max_player_x &&
				input_down(&controls, INPUT_BUTTON_RIGHT) != 0u
			){
				const LevelDefinition *next_level = level_get_next(active_level->id);
				if(next_level != NULL){
					(void)game_load_level(
						next_level->id,
						&active_level,
						&player_entity,
						&game_camera,
						&snapshot
					);
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
			camera_follow(&game_camera, &player_object);
			float max_camera_x = (float)active_level->width - SCREEN_WIDTH;

			if(max_camera_x < 0.0f){
				max_camera_x = 0.0f;
			}
			if(game_camera.x < 0.0f){
				game_camera.x = 0.0f;
			}
			else if(game_camera.x > max_camera_x){
				game_camera.x = max_camera_x;
			}
			game_camera.y = 0.0f;
			snapshot.camera_x = (int16_t)game_camera.x;
			snapshot.camera_y = 0;

			snapshot.level_id = (uint8_t)active_level->id;

			++snapshot.sequence;
			if(protocol_encode(&snapshot, pi_packet) == PROTOCOL_PACKET_SIZE){
				(void)spi_slave_publish(pi_packet);
			}

			game_accumulator -= 1000u;
		}
	}

}
