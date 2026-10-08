#include "statistics.h"

void init_game_stats(struct game_stats *s){
    s->level = 1;
    s->time = 0;
}

void init_player_stats(struct player_stats *s){
    s->lives = 3;
    s->LEDs = 0;
    s->score = 0;
    s->onePower = false;
    s->zeroPower = false;
    s->size = NORMAL;
    s->invincibility = false;
}

void init_enemy_stats(struct enemy_stats *s, enemy_type type){
    switch (type){
        case ENEMY_TYPE_ARDUINO:
            s->hasProjectiles = false;
            s->isAlive = true;
            s->value = 100;
            break;
        case ENEMY_TYPE_ESP:
            s->hasProjectiles = false;
            s->isAlive = true;
            s->value = 200;
            break;
        case ENEMY_TYPE_STM:
            s->hasProjectiles = true;
            s->isAlive = true;
            s->value = 300;
            break;
        default:
            s->hasProjectiles = false;
            s->isAlive = false;
            s->value = 0;
            break;
    }
}
