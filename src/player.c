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
}

void player_update(struct player *p, float delta_time){
    physics_update(p->physics, p->object, delta_time);
}
