#ifndef ENEMY_H
#define ENEMY_H

#include "object.h"
#include "statistics.h"
#include "physics.h"

typedef struct enemy{
    enemy_type type;
    object *object;
    enemy_stats *stats;
    physics *physics;
} enemy;

void enemy_set_type(struct enemy *e, enemy_type type);
enemy_type enemy_get_type(struct enemy *e);
void enemy_set_object(struct enemy *e, struct object *obj);
struct object *enemy_get_object(struct enemy *e);
void enemy_set_physics(struct enemy *e, struct physics *phys);
struct physics *enemy_get_physics(struct enemy *e);
void enemy_set_stats(struct enemy *e, struct enemy_stats *stats);
struct enemy_stats *enemy_get_stats(struct enemy *e);

/* Components must be non-null and outlive the enemy. Initialize object and
 * physics before calling; their configuration is preserved except object type.
 * Enemy statistics are reset for the supplied type. No memory is allocated. */
void enemy_init(struct enemy *e, enemy_type type, struct object *obj,
                struct physics *phys, struct enemy_stats *stats);
void enemy_update(struct enemy *e, float delta_time);

#endif
