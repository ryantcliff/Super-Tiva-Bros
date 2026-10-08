#include "projectile.h"

void projectile_set_type(struct projectile *p, projectile_type type){
    p->type = type;
}

projectile_type projectile_get_type(struct projectile *p){
    return p->type;
}

void projectile_set_object(struct projectile *p, struct object *obj){
    p->object = obj;
    obj->type = OBJECT_TYPE_PROJECTILE;
}

struct object *projectile_get_object(struct projectile *p){
    return p->object;
}

void projectile_set_physics(struct projectile *p, struct physics *phys){
    p->physics = phys;
}

struct physics *projectile_get_physics(struct projectile *p){
    return p->physics;
}

void projectile_set_is_hostile(struct projectile *p, bool is_hostile){
    p->is_hostile = is_hostile;
}

bool projectile_get_is_hostile(struct projectile *p){
    return p->is_hostile;
}

void projectile_init(struct projectile *p, projectile_type type,
                     struct object *obj, struct physics *phys, bool is_hostile){
    projectile_set_type(p, type);
    projectile_set_object(p, obj);
    projectile_set_physics(p, phys);
    projectile_set_is_hostile(p, is_hostile);
}

void projectile_update(struct projectile *p, float delta_time){
    physics_update(p->physics, p->object, delta_time);
}
