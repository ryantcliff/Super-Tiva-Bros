#include "enemy.h"
#include "collision.h"
#define ENEMY_SPEED 32.0f

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
    e->direction = -1; /* start moving left */
    e->physics->acceleration_y = 900.0f;
}

void enemy_update(struct enemy *e, const LevelDefinition *level, float delta_time){
    if (!e->stats->isAlive){
        return;
    }
    if(level==NULL){
        return;
    }
    e->physics->velocity_x = ENEMY_SPEED * e->direction;
    CollisionContacts contacts = collision_move_static(e->object, e->physics, level->solids, level->solid_count, delta_time);
    if (contacts.hit_left){
        e->direction = 1;
    } else if (contacts.hit_right){
        e->direction = -1;
    }

    if (e->object->x <= 0.0f){
        e->direction = 1;
    } else if (e->object->x + e->object->width >= level->width){
        e->direction = -1;
    }

    if (e->object->y > level->height){
        e->stats->isAlive = false;
    }

}
