#ifndef PHYSICS_H
#define PHYSICS_H

#include "object.h"

// Physics properties
typedef struct physics{
    float velocity_x, velocity_y;
    float acceleration_x, acceleration_y;
    float bounce_factor;
} physics;

#endif