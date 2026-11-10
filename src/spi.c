#include "spi.h"

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

#define GPIO_PORTD_CLOCK (1u << 3)
#define SSI2_CLOCK        (1u << 2)
#define SSI2_PINS         0x0Fu
#define SSI2_TX_PIN       (1u << 1)

#define SSI_CR1_SSE (1u << 1)
#define SSI_CR1_MS  (1u << 2)
#define SSI_SR_TNF  (1u << 1)
#define SSI_SR_RNE  (1u << 2)

void spi_slave_init(uint8_t initial_response){
    SYSCTL_RCGCGPIO_R |= GPIO_PORTD_CLOCK;
    SYSCTL_RCGCSSI_R |= SSI2_CLOCK;

    while ((SYSCTL_PRGPIO_R & GPIO_PORTD_CLOCK) == 0u){
    }
    while ((SYSCTL_PRSSI_R & SSI2_CLOCK) == 0u){
    }

    SSI2_CR1_R &= ~SSI_CR1_SSE;

    GPIO_PORTD_AFSEL_R |= SSI2_PINS;
    GPIO_PORTD_PCTL_R =
        (GPIO_PORTD_PCTL_R & ~0x0000FFFFu) | 0x0000FFFFu;
    GPIO_PORTD_AMSEL_R &= ~SSI2_PINS;
    GPIO_PORTD_DEN_R |= SSI2_PINS;
    GPIO_PORTD_DIR_R =
        (GPIO_PORTD_DIR_R & ~SSI2_PINS) | SSI2_TX_PIN;

    /* Eight-bit Freescale SPI, mode 0, with the TM4C acting as slave. */
    SSI2_CR0_R = 0x07u;
    SSI2_CR1_R = SSI_CR1_MS;
    SSI2_CPSR_R = 2u;
    SSI2_CR1_R |= SSI_CR1_SSE;

    while ((SSI2_SR_R & SSI_SR_TNF) == 0u){
    }
    SSI2_DR_R = initial_response;
}

uint32_t spi_slave_service(uint8_t next_response){
    if ((SSI2_SR_R & SSI_SR_RNE) == 0u){
        return 0u;
    }

    (void)SSI2_DR_R;

    while ((SSI2_SR_R & SSI_SR_TNF) == 0u){
    }
    SSI2_DR_R = next_response;

    return 1u;
}
