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

#endif