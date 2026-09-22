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

#endif