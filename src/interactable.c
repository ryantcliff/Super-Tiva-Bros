#include "interactable.h"

void interactable_set_type(struct interactable *i, interactable_type type){
    i->type = type;
}

interactable_type interactable_get_type(struct interactable *i){
    return i->type;
}

void interactable_set_object(struct interactable *i, struct object *obj){
    i->object = obj;
    obj->type = OBJECT_TYPE_INTERACTIVE;
}

struct object *interactable_get_object(struct interactable *i){
    return i->object;
}

void interactable_set_physics(struct interactable *i, struct physics *phys){
    i->physics = phys;
}

struct physics *interactable_get_physics(struct interactable *i){
    return i->physics;
}

void interactable_init(struct interactable *i, interactable_type type,
                       struct object *obj, struct physics *phys){
    interactable_set_type(i, type);
    interactable_set_object(i, obj);
    interactable_set_physics(i, phys);
}

void interactable_update(struct interactable *i, float delta_time){
    physics_update(i->physics, i->object, delta_time);
}
