#ifndef SPI_PACKET_STREAM_H
#define SPI_PACKET_STREAM_H

#include <stddef.h>
#include <stdint.h>
#include "protocol.h"

typedef struct{
    uint8_t packets[2][PROTOCOL_PACKET_SIZE];
    size_t byte_index;
    uint8_t active_packet;
    uint8_t pending_valid;
} SpiPacketStream;

uint32_t spi_packet_stream_init(
    SpiPacketStream *SpiPacketStream,
    const uint8_t initial_packet[PROTOCOL_PACKET_SIZE]
);

uint32_t spi_packet_stream_publish(
    SpiPacketStream *stream,
    const uint8_t packet[PROTOCOL_PACKET_SIZE]
);

uint8_t spi_packet_stream_next_byte(SpiPacketStream *stream);

#endif