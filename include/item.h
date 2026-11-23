#ifndef ITEM_H
#define ITEM_H

#include "object.h"
#include "physics.h"
#include <stdbool.h>

#include "level_data.h"

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


void item_init(struct item *i, item_type type, struct object *obj,
               struct physics *phys);
void item_update(struct item *i, float delta_time);

#endif
