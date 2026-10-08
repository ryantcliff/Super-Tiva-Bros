#ifndef LEVEL_H
#define LEVEL_H

#include "object.h"
#include "physics.h"

typedef struct level {
    object *object;
    physics *physics;
} level;

void level_set_object(struct level *l, struct object *obj);
struct object *level_get_object(struct level *l);
void level_set_physics(struct level *l, struct physics *phys);
struct physics *level_get_physics(struct level *l);

/* Components must be non-null, initialized, and outlive the level.
 * Their configuration is preserved. No memory is allocated. */
void level_init(struct level *l, struct object *obj, struct physics *phys);
void level_update(struct level *l, float delta_time);

#endif
