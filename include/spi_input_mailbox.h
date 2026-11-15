#ifndef SPI_INPUT_MAILBOX_H
#define SPI_INPUT_MAILBOX_H

#include <stdint.h>

typedef struct {
    volatile uint8_t buttons;
    volatile uint32_t generation;
} SpiInputMailbox;

void spi_input_mailbox_init(SpiInputMailbox *mailbox);
void spi_input_mailbox_receive(SpiInputMailbox *mailbox, uint8_t buttons);
uint8_t spi_input_mailbox_buttons(const SpiInputMailbox *mailbox);
uint32_t spi_input_mailbox_generation(const SpiInputMailbox *mailbox);

#endif