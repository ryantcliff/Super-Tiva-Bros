#include "block.h"

void block_set_type(struct block *b, block_type type){
    b->type = type;
}

block_type block_get_type(struct block *b){
    return b->type;
}

void block_set_object(struct block *b, struct object *obj){
    b->object = obj;
    obj->type = OBJECT_TYPE_BLOCK;
}

struct object *block_get_object(struct block *b){
    return b->object;
}

void block_set_physics(struct block *b, struct physics *phys){
    b->physics = phys;
}

struct physics *block_get_physics(struct block *b){
    return b->physics;
}

void block_set_is_permeable(struct block *b, bool is_permeable){
    b->is_permeable = is_permeable;
}

bool block_get_is_permeable(struct block *b){
    return b->is_permeable;
}

void block_init(struct block *b, block_type type, struct object *obj,
                struct physics *phys, bool is_permeable){
    block_set_type(b, type);
    block_set_object(b, obj);
    block_set_physics(b, phys);
    block_set_is_permeable(b, is_permeable);
}

void block_update(struct block *b, float delta_time){
    physics_update(b->physics, b->object, delta_time);
}
