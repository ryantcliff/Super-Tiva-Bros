#include "physics.h"

void physics_set_velocity_x(struct physics *phys, float velocity_x){
    phys->velocity_x = velocity_x;
}

float physics_get_velocity_x(struct physics *phys){
    return phys->velocity_x;
}

void physics_set_velocity_y(struct physics *phys, float velocity_y){
    phys->velocity_y = velocity_y;
}

float physics_get_velocity_y(struct physics *phys){
    return phys->velocity_y;
}

void physics_set_acceleration_x(struct physics *phys, float acceleration_x){
    phys->acceleration_x = acceleration_x;
}

float physics_get_acceleration_x(struct physics *phys){
    return phys->acceleration_x;
}

void physics_set_acceleration_y(struct physics *phys, float acceleration_y){
    phys->acceleration_y = acceleration_y;
}

float physics_get_acceleration_y(struct physics *phys){
    return phys->acceleration_y;
}

void physics_set_bounce_factor(struct physics *phys, float bounce_factor){
    phys->bounce_factor = bounce_factor;
}

float physics_get_bounce_factor(struct physics *phys){
    return phys->bounce_factor;
}

void physics_init(struct physics *phys, float velocity_x, float velocity_y,
                  float acceleration_x, float acceleration_y, float bounce_factor){
    phys->velocity_x = velocity_x;
    phys->velocity_y = velocity_y;
    phys->acceleration_x = acceleration_x;
    phys->acceleration_y = acceleration_y;
    phys->bounce_factor = bounce_factor;
}

void physics_set_velocity(struct physics *phys, float velocity_x, float velocity_y){
    phys->velocity_x = velocity_x;
    phys->velocity_y = velocity_y;
}

void physics_set_acceleration(struct physics *phys, float acceleration_x,
                              float acceleration_y){
    phys->acceleration_x = acceleration_x;
    phys->acceleration_y = acceleration_y;
}

void physics_update(struct physics *phys, struct object *obj, float delta_time){
    // Semi-implicit Euler: integrate velocity before position.
    phys->velocity_x += phys->acceleration_x * delta_time;
    phys->velocity_y += phys->acceleration_y * delta_time;
    obj->x += phys->velocity_x * delta_time;
    obj->y += phys->velocity_y * delta_time;
}
