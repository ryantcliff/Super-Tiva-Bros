#ifndef STATISTICS_H
#define STATISTICS_H

#include "object.h"
#include <stdbool.h>

// Game Stats
typedef struct game_stats{
    int level;
    int time;
} game_stats;

// Player Stats
typedef struct player_stats{
    int lives;
    int LEDs;
    int score;
    bool onePower;
    bool zeroPower;
    enum size{
        BIG,
        NORMAL,
        SMALL
    } size;
    bool invincibility;
} player_stats;

// Enemy Stats
typedef enum {
    ENEMY_TYPE_ARDUINO,
    ENEMY_TYPE_ESP,
    ENEMY_TYPE_STM,
} enemy_type;

typedef struct enemy_stats{
    bool isAlive;
    bool hasProjectiles;
    int value;
} enemy_stats;

void init_game_stats(struct game_stats *s);
void init_player_stats(struct player_stats *s);
void init_enemy_stats(struct enemy_stats *s, enemy_type type);

#endif