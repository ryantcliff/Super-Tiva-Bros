#include "level_data.h"

#define ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

// level-specific geometry belongs here
static const LevelSolid start_solids[] = {
    {0, 208, 512, 16},      //ground
    {176, 176, 48, 16},     //platform
    {320, 160, 64, 16},     //platform
    {272, 176, 16, 32}      //wall
};

// registry of all available levels
static const LevelDefinition levels[] = {
    {
        LEVEL_ID_START,
        512, 224,
        100.0f, 192.0f,
        start_solids,
        ARRAY_COUNT(start_solids)
    }
};

const LevelDefinition *level_get(LevelID id){
    size_t i;
    for(i = 0; i < ARRAY_COUNT(levels); i++){
        if(levels[i].id == id){
            return &levels[i];
        }
    }

    return NULL;
}