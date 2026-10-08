#include "level.h"

void level_set_object(struct level *l, struct object *obj){
    l->object = obj;
}

struct object *level_get_object(struct level *l){
    return l->object;
}

void level_set_physics(struct level *l, struct physics *phys){
    l->physics = phys;
}

struct physics *level_get_physics(struct level *l){
    return l->physics;
}

void level_init(struct level *l, struct object *obj, struct physics *phys){
    level_set_object(l, obj);
    level_set_physics(l, phys);
}

void level_update(struct level *l, float delta_time){
    physics_update(l->physics, l->object, delta_time);
}
