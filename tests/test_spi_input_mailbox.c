#include <assert.h>
#include <stdio.h>

#include "spi_input_mailbox.h"

int main(void)
{
    SpiInputMailbox mailbox;

    spi_input_mailbox_init(&mailbox);

    assert(spi_input_mailbox_buttons(&mailbox) == 0u);
    assert(spi_input_mailbox_generation(&mailbox) == 0u);

    spi_input_mailbox_receive(&mailbox, 0x40u);

    assert(spi_input_mailbox_buttons(&mailbox) == 0x40u);
    assert(spi_input_mailbox_generation(&mailbox) == 1u);

    spi_input_mailbox_receive(&mailbox, 0x80u);

    assert(spi_input_mailbox_buttons(&mailbox) == 0x80u);
    assert(spi_input_mailbox_generation(&mailbox) == 2u);

    puts("spi input mailbox test passed");
    return 0;
}