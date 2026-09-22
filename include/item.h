#ifndef ITEM_H
#define ITEM_H

#include "object.h"
#include "physics.h"
#include <stdbool.h>

typedef enum {
    ITEM_TYPE_LED,
    ITEM_TYPE_BIG_BYTE,
    ITEM_TYPE_LITTLE_BYTE,
    ITEM_TYPE_ONE_FLOWER,
    ITEM_TYPE_ZERO_FLOWER,
    ITEM_TYPE_STATIC_PROTECT,
    ITEM_TYPE_RGB_LED
} item_type;

typedef struct item {
    item_type type;
    object *object;
    physics *physics;
    bool is_collected;
} item;

#endif