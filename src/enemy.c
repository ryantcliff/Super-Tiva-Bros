#include "enemy.h"

void enemy_set_type(struct enemy *e, enemy_type type){
    e->type = type;
}

enemy_type enemy_get_type(struct enemy *e){
    return e->type;
}

void enemy_set_object(struct enemy *e, struct object *obj){
    e->object = obj;
    obj->type = OBJECT_TYPE_ENEMY;
}

struct object *enemy_get_object(struct enemy *e){
    return e->object;
}

void enemy_set_physics(struct enemy *e, struct physics *phys){
    e->physics = phys;
}

struct physics *enemy_get_physics(struct enemy *e){
    return e->physics;
}

void enemy_set_stats(struct enemy *e, struct enemy_stats *stats){
    e->stats = stats;
}

struct enemy_stats *enemy_get_stats(struct enemy *e){
    return e->stats;
}

void enemy_init(struct enemy *e, enemy_type type, struct object *obj,
                struct physics *phys, struct enemy_stats *stats){
    enemy_set_type(e, type);
    enemy_set_object(e, obj);
    enemy_set_physics(e, phys);
    enemy_set_stats(e, stats);
    init_enemy_stats(stats, type);
}

void enemy_update(struct enemy *e, float delta_time){
    if (e->stats->isAlive){
        physics_update(e->physics, e->object, delta_time);
    }
}
