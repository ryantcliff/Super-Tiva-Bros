#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "spi_packet_stream.h"

static void fill_packet(
    uint8_t packet[PROTOCOL_PACKET_SIZE],
    uint8_t starting_value
){
    size_t index;
    for(index = 0u; index < PROTOCOL_PACKET_SIZE; index++){
        packet[index] = starting_value + (uint8_t)index;
    }
}

int main(void){
    SpiPacketStream stream;
    uint8_t first_packet[PROTOCOL_PACKET_SIZE];
    uint8_t second_packet[PROTOCOL_PACKET_SIZE];
    size_t index;
    fill_packet(first_packet, 0x10u);
    fill_packet(second_packet, 0x80u);
    assert(spi_packet_stream_init(&stream, first_packet) == 1u);
    
    for(index = 0u; index < PROTOCOL_PACKET_SIZE; ++index){
        assert(spi_packet_stream_next_byte(&stream) == first_packet[index]);
    }
    // should just repeat the first packet if there's no packet pending
    for(index = 0u; index < PROTOCOL_PACKET_SIZE; ++index){
        assert(spi_packet_stream_next_byte(&stream) == first_packet[index]);
    }

    assert(spi_packet_stream_init(&stream, first_packet) == 1u);
    
    for(index = 0u; index < 5u; ++index){
        assert(spi_packet_stream_next_byte(&stream) == first_packet[index]);
    }

    assert(spi_packet_stream_publish(&stream, second_packet) == 1u);
    // publishing a new packet halfway shouldn't disrupt the one that isn't finished
    for(index = 5u; index < PROTOCOL_PACKET_SIZE; ++index){
        assert(spi_packet_stream_next_byte(&stream) == first_packet[index]);
    }

    for(index = 0u; index < PROTOCOL_PACKET_SIZE; ++index){
        assert(spi_packet_stream_next_byte(&stream) == second_packet[index]);
    }
    assert(spi_packet_stream_publish(&stream, NULL) == 0u);

    puts("spi packet stream test passed");
    return 0;
}