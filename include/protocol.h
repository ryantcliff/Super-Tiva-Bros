#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>
#include <stdint.h>

#define PROTOCOL_MAGIC          0xA5u
#define PROTOCOL_VERSION        1u
#define PROTOCOL_PACKET_SIZE    13u     // temporary

typedef enum{
    PLAYER_IDLE,
    PLAYER_WALKING,
    PLAYER_JUMPING
} PlayerState;

typedef enum{
    FACING_LEFT,
    FACING_RIGHT
} FacingDirection;

typedef struct{
    uint8_t sequence;
    int16_t player_x;
    int16_t player_y;
    int16_t camera_x;
    int16_t camera_y;
    uint8_t player_state;
    uint8_t facing_direction;
} GameSnapshot;

size_t protocol_encode(const GameSnapshot *snapshot, uint8_t packet[PROTOCOL_PACKET_SIZE]);

#endif
