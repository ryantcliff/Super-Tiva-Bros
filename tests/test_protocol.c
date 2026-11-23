#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "protocol.h"

int main(void){
    GameSnapshot snapshot = {
        .sequence = 7u,
        .player_x = 0x1234,
        .player_y = -2,
        .camera_x = 0,
        .camera_y = 256,
        .player_state = PLAYER_WALKING,
        .facing_direction = FACING_RIGHT,
        .level_id = 7u,
    };

    uint8_t packet[PROTOCOL_PACKET_SIZE] = {0};

    const uint8_t expected[PROTOCOL_PACKET_SIZE] = {
        0xA5, 0x02, 0x07,
        0x34, 0x12,
        0xFE, 0xFF,
        0x00, 0x00,
        0x00, 0x01,
        0x01, 0x01,
        0x07
    };
    size_t encoded_size = protocol_encode(&snapshot, packet);

    assert(encoded_size == PROTOCOL_PACKET_SIZE);
    assert(memcmp(packet, expected, PROTOCOL_PACKET_SIZE) == 0);
    assert(protocol_encode(NULL, packet) == 0u);
    assert(protocol_encode(&snapshot, NULL) == 0u);

    puts("Protocol encoding test passed");
    return 0;
}