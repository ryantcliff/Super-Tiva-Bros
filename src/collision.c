#include "collision.h"

void collision_set_object1(struct collision *c, struct object *obj){
    c->object1 = obj;
}

struct object *collision_get_object1(struct collision *c){
    return c->object1;
}

void collision_set_object2(struct collision *c, struct object *obj){
    c->object2 = obj;
}

struct object *collision_get_object2(struct collision *c){
    return c->object2;
}

void collision_init(struct collision *c, struct object *object1,
                    struct object *object2){
    c->object1 = object1;
    c->object2 = object2;
}

bool collision_check(const struct collision *c){
    if (!c || !c->object1 || !c->object2 || c->object1 == c->object2) {
        return false;
    }

    const struct object *a = c->object1;
    const struct object *b = c->object2;

    return a->width > 0 && a->height > 0 &&
           b->width > 0 && b->height > 0 &&
           a->x < b->x + b->width && b->x < a->x + a->width &&
           a->y < b->y + b->height && b->y < a->y + a->height;
}
