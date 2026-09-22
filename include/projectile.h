#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "object.h"
#include "physics.h"
#include <stdbool.h>

typedef enum {
    PROJECTILE_TYPE_ONES,
    PROJECTILE_TYPE_ZEROS,
    PROJECTILE_TYPE_STATIC
} projectile_type;

typedef struct projectile {
    projectile_type type;
    struct object *object;
    struct physics *physics;
    bool is_hostile;
} projectile;

#endif