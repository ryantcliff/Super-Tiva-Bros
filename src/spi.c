#include "spi.h"
#include "spi_packet_stream.h"
#include "spi_input_mailbox.h"

#define HWREG(address) (*((volatile uint32_t *)(address)))

#define SYSCTL_RCGCGPIO_R HWREG(0x400FE608u)
#define SYSCTL_RCGCSSI_R  HWREG(0x400FE61Cu)
#define SYSCTL_PRGPIO_R   HWREG(0x400FEA08u)
#define SYSCTL_PRSSI_R    HWREG(0x400FEA1Cu)

#define GPIO_PORTD_BASE 0x4005B000u
#define GPIO_PORTD_DIR_R   HWREG(GPIO_PORTD_BASE + 0x400u)
#define GPIO_PORTD_AFSEL_R HWREG(GPIO_PORTD_BASE + 0x420u)
#define GPIO_PORTD_DEN_R   HWREG(GPIO_PORTD_BASE + 0x51Cu)
#define GPIO_PORTD_AMSEL_R HWREG(GPIO_PORTD_BASE + 0x528u)
#define GPIO_PORTD_PCTL_R  HWREG(GPIO_PORTD_BASE + 0x52Cu)

#define SSI2_BASE 0x4000A000u
#define SSI2_CR0_R  HWREG(SSI2_BASE + 0x000u)
#define SSI2_CR1_R  HWREG(SSI2_BASE + 0x004u)
#define SSI2_DR_R   HWREG(SSI2_BASE + 0x008u)
#define SSI2_SR_R   HWREG(SSI2_BASE + 0x00Cu)
#define SSI2_CPSR_R HWREG(SSI2_BASE + 0x010u)
#define SSI2_IM_R   HWREG(SSI2_BASE + 0x014u)
#define SSI2_MIS_R  HWREG(SSI2_BASE + 0x01Cu)
#define SSI2_ICR_R  HWREG(SSI2_BASE + 0x020u)

#define NVIC_EN1_R    HWREG(0xE000E104u)
#define NVIC_PRI13_R  HWREG(0xE000E434u)

#define GPIO_PORTD_CLOCK (1u << 3)
#define SSI2_CLOCK        (1u << 2)
#define SSI2_PINS         0x0Fu
#define SSI2_TX_PIN       (1u << 1)

#define SSI_CR1_SSE (1u << 1)
#define SSI_CR1_MS  (1u << 2)
#define SSI_SR_TNF  (1u << 1)
#define SSI_SR_RNE  (1u << 2)
#define SSI_INT_RX_OVERRUN  (1u << 0)
#define SSI_INT_RX_TIMEOUT  (1u << 1)
#define SSI_INT_RX_FIFO     (1u << 2)
#define SSI_INT_TX_FIFO     (1u << 3)

#define SSI_INTERRUPT_MASK  \
    (SSI_INT_RX_OVERRUN |   \
     SSI_INT_RX_TIMEOUT |   \
     SSI_INT_RX_FIFO |      \
     SSI_INT_TX_FIFO)

#define SSI2_NVIC_ENABLE    (1u << 22)

#define SSI2_PRIORITY_MASK  (0x7u << 21)
#define SSI2_PRIORITY_2     (0x2u << 21)

static SpiPacketStream packet_stream;
static SpiInputMailbox input_mailbox;

void spi_slave_init(const uint8_t initial_packet[PROTOCOL_PACKET_SIZE]){
    spi_input_mailbox_init(&input_mailbox);
    if(spi_packet_stream_init(&packet_stream, initial_packet) == 0u){
        return;
    }
    
    SYSCTL_RCGCGPIO_R |= GPIO_PORTD_CLOCK;
    SYSCTL_RCGCSSI_R |= SSI2_CLOCK;

    while ((SYSCTL_PRGPIO_R & GPIO_PORTD_CLOCK) == 0u){
    }
    while ((SYSCTL_PRSSI_R & SSI2_CLOCK) == 0u){
    }

    SSI2_CR1_R &= ~SSI_CR1_SSE;
    SSI2_IM_R = 0u;

    GPIO_PORTD_AFSEL_R |= SSI2_PINS;
    GPIO_PORTD_PCTL_R =
        (GPIO_PORTD_PCTL_R & ~0x0000FFFFu) | 0x0000FFFFu;
    GPIO_PORTD_AMSEL_R &= ~SSI2_PINS;
    GPIO_PORTD_DEN_R |= SSI2_PINS;
    GPIO_PORTD_DIR_R =
        (GPIO_PORTD_DIR_R & ~SSI2_PINS) | SSI2_TX_PIN;

    // 8-bit freescale spi, mode 0, with the tm4c acting as slave
    SSI2_CR0_R = 0x07u;
    SSI2_CR1_R = SSI_CR1_MS;
    SSI2_CPSR_R = 2u;

    while ((SSI2_SR_R & SSI_SR_TNF) == 0u){
        (void)SSI2_DR_R;
    }
    SSI2_ICR_R = SSI_INT_RX_OVERRUN | SSI_INT_RX_TIMEOUT;

    while((SSI2_SR_R & SSI_SR_TNF) != 0){
        SSI2_DR_R = spi_packet_stream_next_byte(&packet_stream);
    }

    NVIC_PRI13_R = (NVIC_PRI13_R & ~SSI2_PRIORITY_MASK) | SSI2_PRIORITY_2;
    NVIC_EN1_R = SSI2_NVIC_ENABLE;
    SSI2_IM_R = SSI_INTERRUPT_MASK;
    SSI2_CR1_R |= SSI_CR1_SSE;
}

uint32_t spi_slave_publish(const uint8_t packet[PROTOCOL_PACKET_SIZE]){
    uint32_t previous_interrupt_mask;
    uint32_t result;
    if(packet == NULL){
        return 0u;
    }
    previous_interrupt_mask = SSI2_IM_R;
    SSI2_IM_R = 0u;

    result = spi_packet_stream_publish(&packet_stream, packet);

    SSI2_IM_R = previous_interrupt_mask;

    return result;
}

uint8_t spi_slave_input_read(void){
    return spi_input_mailbox_buttons(&input_mailbox);
}

uint32_t spi_slave_input_generation(void){
    return spi_input_mailbox_generation(&input_mailbox);
}

void SSI2_Handler(void){
    uint32_t interrupt_status;
    interrupt_status = SSI2_MIS_R;
    while ((SSI2_SR_R & SSI_SR_RNE) != 0u) {
        uint8_t received_byte = (uint8_t)(SSI2_DR_R & 0xFFu);

        spi_input_mailbox_receive(
            &input_mailbox,
            received_byte
        );
    }
    SSI2_ICR_R = interrupt_status & (SSI_INT_RX_OVERRUN | SSI_INT_RX_TIMEOUT);
    while((SSI2_SR_R & SSI_SR_TNF) != 0u){
        SSI2_DR_R = spi_packet_stream_next_byte(&packet_stream);
    }
}
