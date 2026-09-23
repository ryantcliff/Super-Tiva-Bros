#ifndef PHYSICS_H
#define PHYSICS_H

#include "object.h"

// Physics properties
typedef struct physics{
    float velocity_x, velocity_y;
    float acceleration_x, acceleration_y;
    float bounce_factor;
} physics;

void physics_set_velocity_x(struct physics *phys, float velocity_x);
float physics_get_velocity_x(struct physics *phys);
void physics_set_velocity_y(struct physics *phys, float velocity_y);
float physics_get_velocity_y(struct physics *phys);
void physics_set_acceleration_x(struct physics *phys, float acceleration_x);
float physics_get_acceleration_x(struct physics *phys);
void physics_set_acceleration_y(struct physics *phys, float acceleration_y);
float physics_get_acceleration_y(struct physics *phys);
void physics_set_bounce_factor(struct physics *phys, float bounce_factor);
float physics_get_bounce_factor(struct physics *phys);
void physics_init(struct physics *phys, float velocity_x, float velocity_y,
                  float acceleration_x, float acceleration_y, float bounce_factor);
void physics_set_velocity(struct physics *phys, float velocity_x, float velocity_y);
void physics_set_acceleration(struct physics *phys, float acceleration_x,
                              float acceleration_y);
void physics_update(struct physics *phys, struct object *obj, float delta_time);

#endif
