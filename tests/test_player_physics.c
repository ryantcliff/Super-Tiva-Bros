#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "object.h"
#include "physics.h"
#include "player.h"
#include "statistics.h"

static bool float_near(float actual, float expected){
    float difference = actual - expected;
    if(difference < 0.0f){
        difference = -difference;
    }
    return difference < 0.001f;
}

int main(void){
    object player_object;
    physics player_physics;
    player_stats stats;
    player test_player;

    object_init(&player_object, 100.0f, 192.0f, 16, 16, OBJECT_TYPE_PLAYER);
    physics_init(&player_physics, 0.0f, 0.0f, 0.0f, 900.0f, 0.0f);
    player_init(&test_player, &player_object, &player_physics, &stats);

    assert(!player_is_grounded(&test_player));

    player_resolve_floor(&test_player, 192.0f);

    assert(player_is_grounded(&test_player));
    assert(player_object.y == 192.0f);
    assert(player_physics.velocity_y == 0.0f);
    assert(!player_try_jump(&test_player, 0.0f));
    assert(player_is_grounded(&test_player));
    assert(player_try_jump(&test_player, 300.0f));
    assert(!player_is_grounded(&test_player));
    assert(!player_try_jump(&test_player, 300.0f));

    player_resolve_floor(&test_player, 192.0f);
    assert(!player_is_grounded(&test_player));
    assert(player_physics.velocity_y == -300.0f);
    assert(!player_try_jump(&test_player, 300.0f));
    assert(player_physics.velocity_y == -300.0f);
    player_object.y = 200.0f;
    player_physics.velocity_y = 50.0f;

    player_resolve_floor(&test_player, 192.0f);
    assert(player_is_grounded(&test_player));
    assert(player_object.y == 192.0f);
    assert(player_physics.velocity_y == 0.0f);

    player_object.x = 100.0f;
    player_object.y = 192.0f;
    physics_set_velocity_x(&player_physics, 60.0f);
    assert(player_try_jump(&test_player, 300.0f));
    player_update(&test_player, 1.0f/60.0f);
    player_resolve_floor(&test_player, 192.0f);
    assert(float_near(player_object.x, 101.0f));
    assert(float_near(player_object.y, 187.25f));
    assert(float_near(player_physics.velocity_y, -285.0f));
    assert(!player_is_grounded(&test_player));

    // ground deceleration moves velocity toward zero
    player_object.y = 192.0f;
    player_physics.velocity_y = 0.0f;
    player_resolve_floor(&test_player, 192.0f);

    physics_set_velocity_x(&player_physics, 60.0f);
    player_apply_ground_deceleration(
        &test_player,
        300.0f,
        1.0f / 60.0f
    );
    assert(float_near(player_physics.velocity_x, 55.0f));

    physics_set_velocity_x(&player_physics, -60.0f);
    player_apply_ground_deceleration(
        &test_player,
        300.0f,
        1.0f / 60.0f
    );
    assert(float_near(player_physics.velocity_x, -55.0f));

    physics_set_velocity_x(&player_physics, 3.0f);
    player_apply_ground_deceleration(
        &test_player,
        300.0f,
        1.0f / 60.0f
    );
    assert(float_near(player_physics.velocity_x, 0.0f));

    physics_set_velocity_x(&player_physics, -3.0f);
    player_apply_ground_deceleration(
        &test_player,
        300.0f,
        1.0f / 60.0f
    );
    assert(float_near(player_physics.velocity_x, 0.0f));

    physics_set_velocity_x(&player_physics, 60.0f);

    player_apply_ground_deceleration(
        &test_player,
        0.0f,
        1.0f / 60.0f
    );
    assert(float_near(player_physics.velocity_x, 60.0f));

    player_apply_ground_deceleration(
        &test_player,
        300.0f,
        0.0f
    );
    assert(float_near(player_physics.velocity_x, 60.0f));

    assert(player_try_jump(&test_player, 300.0f));
    physics_set_velocity_x(&player_physics, 60.0f);
    player_apply_ground_deceleration(
        &test_player,
        300.0f,
        1.0f / 60.0f
    );
    assert(float_near(player_physics.velocity_x, 60.0f));

    puts("player physics test passed");
    return 0;
}