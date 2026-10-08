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

void item_set_type(struct item *i, item_type type);
item_type item_get_type(struct item *i);
void item_set_object(struct item *i, struct object *obj);
struct object *item_get_object(struct item *i);
void item_set_physics(struct item *i, struct physics *phys);
struct physics *item_get_physics(struct item *i);
void item_set_is_collected(struct item *i, bool is_collected);
bool item_get_is_collected(struct item *i);

/* Components must be non-null and outlive the item. Initialize object and
 * physics before calling; their configuration is preserved except object type.
 * The item starts uncollected. No memory is allocated. */
void item_init(struct item *i, item_type type, struct object *obj,
               struct physics *phys);
void item_update(struct item *i, float delta_time);

#endif
