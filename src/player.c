#include "player.h"

void player_set_object(struct player *p, struct object *obj){
    p->object = obj;
    obj->type = OBJECT_TYPE_PLAYER;
}

struct object *player_get_object(struct player *p){
    return p->object;
}

void player_set_physics(struct player *p, struct physics *phys){
    p->physics = phys;
}

struct physics *player_get_physics(struct player *p){
    return p->physics;
}

void player_set_stats(struct player *p, struct player_stats *stats){
    p->stats = stats;
}

struct player_stats *player_get_stats(struct player *p){
    return p->stats;
}

void player_init(struct player *p, struct object *obj, struct physics *phys,
                 struct player_stats *stats){
    player_set_object(p, obj);
    player_set_physics(p, phys);
    player_set_stats(p, stats);
    init_player_stats(stats);
    p->grounded = false;
}

void player_update(struct player *p, float delta_time){
    physics_update(p->physics, p->object, delta_time);
}

bool player_is_grounded(const struct player *p){
    return p->grounded;
}

bool player_try_jump(struct player *p, float jump_speed){
    if(!p->grounded || !(jump_speed > 0.0f)){
        return false;
    }
    p->physics->velocity_y = -jump_speed;
    p->grounded = false;
    return true;
}

void player_resolve_floor(struct player *p, float floor_y){
    if((p->object->y >= floor_y) && (p->physics->velocity_y >= 0.0f)){
        p->object->y = floor_y;
        p->physics->velocity_y = 0.0f;
        p->grounded = true;
    }
    else{
        p->grounded = false;
    }
}

void player_apply_ground_deceleration(struct player *p, float deceleration, float delta_time){
    float velocity_x;
    float velocity_change;

    if(
        !p->grounded ||
        !(deceleration > 0.0f) ||
        !(delta_time > 0.0f)
    ){
        return;
    }

    velocity_x = p->physics->velocity_x;
    velocity_change = deceleration * delta_time;

    if(velocity_x > 0.0f){
        if(velocity_x <= velocity_change){
            p->physics->velocity_x = 0.0f;
        }
        else{
            p->physics->velocity_x = velocity_x - velocity_change;
        }
    }
    else if(velocity_x < 0.0f){
        if(-velocity_x <= velocity_change){
            p->physics->velocity_x = 0.0f;
        }
        else{
            p->physics->velocity_x = velocity_x + velocity_change;
        }
    }
}