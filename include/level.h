#ifndef LEVEL_H
#define LEVEL_H

#include "object.h"
#include "physics.h"

typedef struct level {
    object *object;
    physics *physics;
} level;

#endif