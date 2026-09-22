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

#endif