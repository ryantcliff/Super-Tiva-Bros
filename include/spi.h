#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "protocol.h"

void spi_slave_init(const uint8_t initial_packet[PROTOCOL_PACKET_SIZE]);
uint32_t spi_slave_publish(const uint8_t packet[PROTOCOL_PACKET_SIZE]);
uint8_t spi_slave_input_read(void);
uint32_t spi_slave_input_generation(void);
void SSI2_Handler(void);

#endif
