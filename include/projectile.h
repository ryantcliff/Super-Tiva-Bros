#ifndef PROJECTILES_H
#define PROJECTILES_H

#include "object.h"
#include "physics.h"
#include <stdbool.h>

typedef enum {
    PROJECTILE_TYPE_ONES,
    PROJECTILE_TYPE_ZEROS,
    PROJECTILE_TYPE_STATIC
} projectile_type;

typedef struct projectile {
    projectile_type type;
    struct object *object;
    struct physics *physics;
    bool is_hostile;
} projectile;

void projectile_set_type(struct projectile *p, projectile_type type);
projectile_type projectile_get_type(struct projectile *p);
void projectile_set_object(struct projectile *p, struct object *obj);
struct object *projectile_get_object(struct projectile *p);
void projectile_set_physics(struct projectile *p, struct physics *phys);
struct physics *projectile_get_physics(struct projectile *p);
void projectile_set_is_hostile(struct projectile *p, bool is_hostile);
bool projectile_get_is_hostile(struct projectile *p);

/* Components must be non-null and outlive the projectile. Initialize object and
 * physics before calling; their configuration is preserved except object type.
 * No memory is allocated. */
void projectile_init(struct projectile *p, projectile_type type,
                     struct object *obj, struct physics *phys, bool is_hostile);
void projectile_update(struct projectile *p, float delta_time);

#endif
