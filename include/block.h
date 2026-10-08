#ifndef BLOCK_H
#define BLOCK_H

#include "object.h"
#include "physics.h"
#include <stdbool.h>

typedef enum {
    BLOCK_TYPE_BRICK,
    BLOCK_TYPE_COBBLED,
    BLOCK_TYPE_CHISELED,
    BLOCK_TYPE_QUESTION,
    BLOCK_TYPE_EMPTY,
    BLOCK_TYPE_PIPE_TOP,
    BLOCK_TYPE_PIPE_BOTTOM,
    BLOCK_TYPE_BRIDGE,
    BLOCK_TYPE_PLATFORM
} block_type;

// Block Structure
typedef struct block{
    block_type type;
    object *object;
    physics *physics;
    bool is_permeable;
} block;

void block_set_type(struct block *b, block_type type);
block_type block_get_type(struct block *b);
void block_set_object(struct block *b, struct object *obj);
struct object *block_get_object(struct block *b);
void block_set_physics(struct block *b, struct physics *phys);
struct physics *block_get_physics(struct block *b);
void block_set_is_permeable(struct block *b, bool is_permeable);
bool block_get_is_permeable(struct block *b);

/* Components must be non-null, initialized, and outlive the block.
 * Their configuration is preserved except object type. No memory is allocated. */
void block_init(struct block *b, block_type type, struct object *obj,
                struct physics *phys, bool is_permeable);
void block_update(struct block *b, float delta_time);

#endif
