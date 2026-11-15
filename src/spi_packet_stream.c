#include "spi_packet_stream.h"

static void copy_packet(
    uint8_t destination[PROTOCOL_PACKET_SIZE],
    const uint8_t source[PROTOCOL_PACKET_SIZE]
){
    size_t index;
    for(index = 0u; index < PROTOCOL_PACKET_SIZE; ++index){
        destination[index] = source[index];
    }
}

uint32_t spi_packet_stream_init(
    SpiPacketStream *stream,
    const uint8_t initial_packet[PROTOCOL_PACKET_SIZE]
){
    if((stream == NULL) || (initial_packet == NULL)){
        return 0u;
    }
    copy_packet(stream->packets[0], initial_packet);
    copy_packet(stream->packets[1], initial_packet);
    stream->byte_index = 0u;
    stream->active_packet = 0u;
    stream->pending_valid = 0u;

    return 1u;
}

uint32_t spi_packet_stream_publish(
    SpiPacketStream *stream,
    const uint8_t packet[PROTOCOL_PACKET_SIZE]
){
    uint8_t pending_packet;
    if((stream == NULL) || (packet == NULL)){
        return 0u;
    }

    pending_packet = stream->active_packet ^ 1u;
    copy_packet(stream->packets[pending_packet], packet);
    stream->pending_valid = 1u;

    return 1u;
}

uint8_t spi_packet_stream_next_byte(SpiPacketStream *stream){
    uint8_t value;

    if(stream == NULL){
        return 0u;
    }

    value = stream->packets[stream->active_packet][stream->byte_index];
    ++stream->byte_index;
    if(stream->byte_index >= PROTOCOL_PACKET_SIZE){
        stream->byte_index = 0u;
        if(stream->pending_valid != 0){
            stream->active_packet ^= 1u;
            stream->pending_valid = 0u;
        }
    }
    return value;
}