#ifndef GAME_H
#define GAME_H

#include <stddef.h>

#include "player.h"
#include "interactable.h"
#include "block.h"
#include "item.h"
#include "level.h"
#include "enemy.h"
#include "projectile.h"

typedef struct game {
    player *player;
    interactable **interactables;
    block **blocks;
    item **items;
    level *level;
    enemy **enemies;
    size_t interactable_count;
    size_t block_count;
    size_t item_count;
    size_t enemy_count;
    projectile **projectiles;
    size_t projectile_count;
} game;

/* State must be non-null. Entities must already be initialized and, together
 * with their arrays, outlive the game. Player and level may be null; arrays
 * may be null only when their count is zero. Entries must be non-null.
 * Initialization preserves entity state and performs no allocation. */
void game_init(game *state, player *p, level *l,
               interactable **interactables, size_t interactable_count,
               block **blocks, size_t block_count,
               item **items, size_t item_count,
               enemy **enemies, size_t enemy_count,
               projectile **projectiles, size_t projectile_count);
/* Advance each entity once. Delta time is in seconds; non-positive and
 * non-finite durations are ignored. */
void game_update(game *state, float delta_time);

#endif
