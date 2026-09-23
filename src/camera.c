#include "camera.h"

void camera_init(struct camera *cam, float x, float y, int width, int height){
    cam->x = x;
    cam->y = y;
    cam->width = width;
    cam->height = height;
}

void camera_set_position(struct camera *cam, float x, float y){
    cam->x = x;
    cam->y = y;
}

void camera_move(struct camera *cam, float dx, float dy){
    cam->x += dx;
    cam->y += dy;
}

void camera_follow(struct camera *cam, const struct object *target){
    cam->x = target->x + 0.5f * target->width - 0.5f * cam->width;
    cam->y = target->y + 0.5f * target->height - 0.5f * cam->height;
}

void camera_world_to_screen(const struct camera *cam, float world_x,
                            float world_y, float *screen_x, float *screen_y){
    *screen_x = world_x - cam->x;
    *screen_y = world_y - cam->y;
}

void camera_screen_to_world(const struct camera *cam, float screen_x,
                            float screen_y, float *world_x, float *world_y){
    *world_x = screen_x + cam->x;
    *world_y = screen_y + cam->y;
}

bool camera_is_visible(const struct camera *cam, const struct object *obj){
    return obj->width > 0 && obj->height > 0 &&
           obj->x < cam->x + cam->width &&
           obj->x + obj->width > cam->x &&
           obj->y < cam->y + cam->height &&
           obj->y + obj->height > cam->y;
}
