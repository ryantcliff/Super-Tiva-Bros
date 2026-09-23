#include "game.h"

#include <float.h>

void game_init(game *state, player *p, level *l,
               interactable **interactables, size_t interactable_count,
               block **blocks, size_t block_count,
               item **items, size_t item_count,
               enemy **enemies, size_t enemy_count,
               projectile **projectiles, size_t projectile_count){
    state->player = p;
    state->level = l;
    state->interactables = interactables;
    state->interactable_count = interactable_count;
    state->blocks = blocks;
    state->block_count = block_count;
    state->items = items;
    state->item_count = item_count;
    state->enemies = enemies;
    state->enemy_count = enemy_count;
    state->projectiles = projectiles;
    state->projectile_count = projectile_count;
}

void game_update(game *state, float delta_time){
    size_t i;

    if (!(delta_time > 0.0f && delta_time <= FLT_MAX)){
        return;
    }

    if (state->level){
        level_update(state->level, delta_time);
    }
    if (state->player){
        player_update(state->player, delta_time);
    }
    for (i = 0; i < state->interactable_count; ++i){
        interactable_update(state->interactables[i], delta_time);
    }
    for (i = 0; i < state->block_count; ++i){
        block_update(state->blocks[i], delta_time);
    }
    for (i = 0; i < state->item_count; ++i){
        item_update(state->items[i], delta_time);
    }
    for (i = 0; i < state->enemy_count; ++i){
        enemy_update(state->enemies[i], delta_time);
    }
    for (i = 0; i < state->projectile_count; ++i){
        projectile_update(state->projectiles[i], delta_time);
    }
}
