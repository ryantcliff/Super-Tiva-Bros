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

CollisionContacts collision_move_static(
    object *body,
    physics *motion,
    const LevelSolid *solids,
    size_t solid_count,
    float dt
){
    CollisionContacts contacts = {
        false, false, false, false
    };

    size_t i;
    float old_x, next_x, old_y, next_y;

    if(!body || !motion ||
        (!solids && solid_count != 0u) ||
        body->width <= 0 || body->height <= 0 ||
        !(dt > 0.0f)
    ){
        return contacts;
    }

    motion->velocity_x += motion->acceleration_x * dt;
    motion->velocity_y += motion->acceleration_y * dt;

    old_x = body->x;
    next_x = old_x + motion->velocity_x * dt;

    for(i = 0; i < solid_count; i++){
        const LevelSolid *s = &solids[i];
        float right = (float)s->x + s->width;
        float bottom = (float)s->y + s->height;

        if(s->width == 0u || s->height == 0u ||
            body->y + body->height <= s->y ||
            body->y >= bottom
        ){
            continue;
        }

        if(motion->velocity_x > 0.0f &&
            old_x + body->width <= s->x &&
            next_x + body->width >= s->x
        ){
            next_x = (float)s->x - body->width;
            contacts.hit_right = true;
        }
        else if(motion->velocity_x < 0.0f &&
            old_x >= right &&
            next_x <= right
        ){
            next_x = right;
            contacts.hit_left = true;
        }
    }

    body->x = next_x;

    if(contacts.hit_left || contacts.hit_right){
        motion->velocity_x = 0.0f;
    }

    old_y = body->y;
    next_y = old_y + motion->velocity_y * dt;

    for(i = 0; i < solid_count; i++){
        const LevelSolid *s = &solids[i];
        float right = (float)s->x + s->width;
        float bottom = (float)s->y + s->height;

        if(s->width == 0u || s->height == 0u ||
            body->x + body->width <= s->x ||
            body->x >= right
        ){
            continue;
        }

        if(motion->velocity_y >= 0.0f &&
            old_y + body->height <= s->y &&
            next_y + body->height >= s->y
        ){
            next_y = (float)s->y - body->height;
            contacts.grounded = true;
        }
        else if( motion->velocity_y < 0.0f &&
            old_y >= bottom &&
            next_y <= bottom
        ){
            next_y = bottom;
            contacts.hit_ceiling = true;
        }
    }

    body->y = next_y;

    if(contacts.grounded || contacts.hit_ceiling){
        motion->velocity_y = 0.0f;
    }

    return contacts;
}