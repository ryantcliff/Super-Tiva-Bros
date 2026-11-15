#include "spi_input_mailbox.h"

void spi_input_mailbox_init(SpiInputMailbox *mailbox){
    mailbox->buttons = 0u;
    mailbox->generation = 0u;
}

void spi_input_mailbox_receive(SpiInputMailbox *mailbox, uint8_t buttons){
    mailbox->buttons = buttons;
    mailbox->generation += 1u;
}

uint8_t spi_input_mailbox_buttons(const SpiInputMailbox *mailbox){
    return mailbox->buttons;
}

uint32_t spi_input_mailbox_generation(const SpiInputMailbox *mailbox){
    return mailbox->generation;
}