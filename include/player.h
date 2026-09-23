#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "physics.h"
#include "statistics.h"

typedef struct player {
    object *object;
    physics *physics;
    player_stats *stats;
} player;

void player_set_object(struct player *p, struct object *obj);
struct object *player_get_object(struct player *p);
void player_set_physics(struct player *p, struct physics *phys);
struct physics *player_get_physics(struct player *p);
void player_set_stats(struct player *p, struct player_stats *stats);
struct player_stats *player_get_stats(struct player *p);

/* Components must be non-null and outlive the player. Initialize object and
 * physics before calling; their configuration is preserved except object type.
 * Player statistics are reset to their starting values. No memory is allocated. */
void player_init(struct player *p, struct object *obj, struct physics *phys,
                 struct player_stats *stats);
void player_update(struct player *p, float delta_time);

#endif
