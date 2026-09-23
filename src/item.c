#include "item.h"

void item_set_type(struct item *i, item_type type){
    i->type = type;
}

item_type item_get_type(struct item *i){
    return i->type;
}

void item_set_object(struct item *i, struct object *obj){
    i->object = obj;
    obj->type = OBJECT_TYPE_ITEM;
}

struct object *item_get_object(struct item *i){
    return i->object;
}

void item_set_physics(struct item *i, struct physics *phys){
    i->physics = phys;
}

struct physics *item_get_physics(struct item *i){
    return i->physics;
}

void item_set_is_collected(struct item *i, bool is_collected){
    i->is_collected = is_collected;
}

bool item_get_is_collected(struct item *i){
    return i->is_collected;
}

void item_init(struct item *i, item_type type, struct object *obj,
               struct physics *phys){
    item_set_type(i, type);
    item_set_object(i, obj);
    item_set_physics(i, phys);
    item_set_is_collected(i, false);
}

void item_update(struct item *i, float delta_time){
    if (!i->is_collected){
        physics_update(i->physics, i->object, delta_time);
    }
}
