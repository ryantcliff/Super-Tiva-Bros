#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include "object.h"
#include "physics.h"

typedef enum {
    INTERACTABLE_TYPE_DOOR,
    INTERACTABLE_TYPE_PIPE_HOLE,
    INTERACTABLE_TYPE_FLAG,
    INTERACTABLE_TYPE_AXE,
    INTERACTABLE_TYPE_PLATFORM
} interactable_type;

typedef struct interactable {
    interactable_type type;
    object *object;
    physics *physics;
} interactable;

void interactable_set_type(struct interactable *i, interactable_type type);
interactable_type interactable_get_type(struct interactable *i);
void interactable_set_object(struct interactable *i, struct object *obj);
struct object *interactable_get_object(struct interactable *i);
void interactable_set_physics(struct interactable *i, struct physics *phys);
struct physics *interactable_get_physics(struct interactable *i);

/* Components must be non-null, initialized, and outlive the interactable.
 * Their configuration is preserved except object type. No memory is allocated. */
void interactable_init(struct interactable *i, interactable_type type,
                       struct object *obj, struct physics *phys);
void interactable_update(struct interactable *i, float delta_time);

#endif
