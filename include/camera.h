#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

typedef struct camera {
    float x, y;
    int width, height;
} camera;

/* Position is the viewport's top-left world coordinate. Dimensions must be
 * positive; all pointer arguments must be non-null. No memory is allocated. */
void camera_init(struct camera *cam, float x, float y, int width, int height);
void camera_set_position(struct camera *cam, float x, float y);
void camera_move(struct camera *cam, float dx, float dy);
void camera_follow(struct camera *cam, const struct object *target);
void camera_world_to_screen(const struct camera *cam, float world_x,
                            float world_y, float *screen_x, float *screen_y);
void camera_screen_to_world(const struct camera *cam, float screen_x,
                            float screen_y, float *world_x, float *world_y);
bool camera_is_visible(const struct camera *cam, const struct object *obj);

#endif
