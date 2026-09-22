#include "object.h"

void object_set_x(struct object *obj, float x){
    obj->x = x;
}

float object_get_x(struct object *obj){
    return obj->x;
}

void object_set_y(struct object *obj, float y){
    obj->y = y;
}

float object_get_y(struct object *obj){
    return obj->y;
}

void object_set_width(struct object *obj, int width){
    obj->width = width;
}

int object_get_width(struct object *obj){
    return obj->width;
}

void object_set_height(struct object *obj, int height){
    obj->height = height;
}

int object_get_height(struct object *obj){
    return obj->height;
}

void object_set_type(struct object *obj, object_type type){
    obj->type = type;
}

object_type object_get_type(struct object *obj){
    return obj->type;
}




