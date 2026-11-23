#include <assert.h>
#include <stdio.h>
#include "level_data.h"
#include "collision.h"
#include "camera.h"
#include "object.h"
#include "physics.h"

int main(void){
    const LevelDefinition *level = level_get(LEVEL_ID_START);
    object body;
    physics motion;
    CollisionContacts contacts;
    camera view;

    assert(level != NULL);
    assert(level->width == 512);
    assert(level->solid_count == 4);

    // test 1: landing on platform
    object_init(&body, 190, 150, 16, 16, OBJECT_TYPE_PLAYER);
    physics_init(&motion, 0, 200, 0, 0, 0);

    contacts = collision_move_static(&body, &motion, level->solids, level->solid_count, 0.1f);

    assert(contacts.grounded);
    assert(body.y == 160.0f);
    assert(motion.velocity_y == 0.0f);

    // test 2: hitting brick bottom
    object_init(&body, 190, 192, 16, 16, OBJECT_TYPE_PLAYER);
    physics_init(&motion, 0, -300, 0, 0, 0);

    contacts = collision_move_static(&body, &motion, level->solids, level->solid_count, 0.1f);

    assert(contacts.hit_ceiling);
    assert(body.y == 192.0f);

    // test 3: walking into a wall
    object_init(&body, 244, 192, 16, 16, OBJECT_TYPE_PLAYER);
    physics_init(&motion, 100, 0, 0, 0, 0);

    contacts = collision_move_static(&body, &motion, level->solids, level->solid_count, 0.2f);

    assert(contacts.hit_right);
    assert(body.x == 256.0f);
    assert(motion.velocity_x == 0.0f);

    // test 4: camera follows player
    camera_init(&view, 0, 0, 256, 224);
    object_set_position(&body, 300, 192);
    camera_follow(&view, &body);

    assert(view.x == 180.0f);

    puts("all level system tests pass");
    return 0;
}