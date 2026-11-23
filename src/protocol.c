#include "protocol.h"

static void insert_int16(
    uint8_t *packet,
    size_t offset,
    int16_t value
){
    uint16_t encoded_value = (uint16_t)value;
    packet[offset] = (uint8_t)(encoded_value & 0xFFu);
    packet[offset + 1u] = (uint8_t)((encoded_value >> 8u & 0xFFu));
}

size_t protocol_encode(
    const GameSnapshot *snapshot,
    uint8_t packet[PROTOCOL_PACKET_SIZE]
){
    if((snapshot == NULL) || (packet == NULL)){
        return 0u;
    }
    // header
    packet[0] = PROTOCOL_MAGIC;
    packet[1] = PROTOCOL_VERSION;
    packet[2] = snapshot->sequence;

    // player and cam coords
    insert_int16(packet, 3u, snapshot->player_x);
    insert_int16(packet, 5u, snapshot->player_y);
    insert_int16(packet, 7u, snapshot->camera_x);
    insert_int16(packet, 9u, snapshot->camera_y);

    // player flags
    packet[11] = snapshot->player_state;
    packet[12] = snapshot->facing_direction;
    
    packet[13] = snapshot->level_id;

    // todo: enemies and other stuff
    
    return PROTOCOL_PACKET_SIZE;
}