#ifndef SPI_H
#define SPI_H

#include <stdint.h>

void spi_slave_init(uint8_t initial_response);
uint32_t spi_slave_service(uint8_t next_response);

#endif
