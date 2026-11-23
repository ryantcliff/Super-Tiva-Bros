#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

#include <stddef.h>
#include <stdint.h>

typedef enum{
    LEVEL_ID_START = 0
} LevelID;

typedef struct{
    int16_t x;
    int16_t y;
    uint16_t width;
    uint16_t height;
} LevelSolid;

typedef struct{
    LevelID id;
    uint16_t width;
    uint16_t height;
    float spawn_x;
    float spawn_y;
    const LevelSolid *solids;
    size_t solid_count;
} LevelDefinition;

const LevelDefinition *level_get(LevelID id);

#endif