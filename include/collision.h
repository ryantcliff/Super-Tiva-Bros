#ifndef COLLISION_H
#define COLLISION_H

#include "object.h"

typedef struct collision {
    object *object1;
    object *object2;
} collision;

void collision_set_object1(struct collision *c, struct object *obj);
struct object *collision_get_object1(struct collision *c);
void collision_set_object2(struct collision *c, struct object *obj);
struct object *collision_get_object2(struct collision *c);
void collision_init(struct collision *c, struct object *object1,
                    struct object *object2);

/* Test axis-aligned bounds. Touching edges, non-positive sizes, missing
 * objects, and self-collisions do not count as overlaps. */
bool collision_check(const struct collision *c);

#include <stddef.h>
#include "physics.h"
#include "level_data.h"

typedef struct{
    bool grounded;
    bool hit_ceiling;
    bool hit_left;
    bool hit_right;
} CollisionContacts;

CollisionContacts collision_move_static(
    object *body,
    physics *motion,
    const LevelSolid *solids,
    size_t solid_count,
    float delta_time
);

#endif
