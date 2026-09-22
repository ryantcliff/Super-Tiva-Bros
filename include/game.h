#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "interactable.h"
#include "block.h"
#include "item.h"
#include "level.h"
#include "enemy.h"

typedef struct game {
    player *player;
    interactable **interactables;
    block **blocks;
    item **items;
    level *level;
    enemy **enemies;
} game;

#endif