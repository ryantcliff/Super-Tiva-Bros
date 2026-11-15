#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "protocol.h"

void spi_slave_init(const uint8_t initial_packet[PROTOCOL_PACKET_SIZE]);
uint32_t spi_slave_publish(const uint8_t packet[PROTOCOL_PACKET_SIZE]);
void SSI2_Handler(void);

#endif
