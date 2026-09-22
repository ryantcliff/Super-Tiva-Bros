#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

typedef enum {
        OBJECT_TYPE_PLAYER,
        OBJECT_TYPE_ENEMY,
        OBJECT_TYPE_ITEM,
        OBJECT_TYPE_PROJECTILE,
        OBJECT_TYPE_INTERACTIVE,
        OBJECT_TYPE_BLOCK
    } object_type;

typedef struct object{
    float x, y;
    int width, height;
    object_type type;
} object;

void object_set_x(struct object *obj, float x);
float object_get_x(struct object *obj);
void object_set_y(struct object *obj, float y);
float object_get_y(struct object *obj);
void object_set_width(struct object *obj, int width);
int object_get_width(struct object *obj);
void object_set_height(struct object *obj, int height);
int object_get_height(struct object *obj);
void object_set_type(struct object *obj, object_type type);
object_type object_get_type(struct object *obj);

#endif