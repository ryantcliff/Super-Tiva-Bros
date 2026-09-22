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

#endif