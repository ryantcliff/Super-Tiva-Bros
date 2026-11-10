#include "uart.h"

#define HWREG(address) (*((volatile uint32_t *)(address)))

#define UART0_DR_R HWREG(0x4000C000u)
#define UART0_FR_R HWREG(0x4000C018u)

#define UART_FR_RXFE 0x10u
#define UART_FR_TXFF 0x20u

void uart_write_char(char character){
    while ((UART0_FR_R & UART_FR_TXFF) != 0u){
    }
    UART0_DR_R = (uint32_t)(uint8_t)character;
}

void uart_write_string(const char *text){
    while (*text != '\0'){
        uart_write_char(*text++);
    }
}

int32_t uart_read_char(void){
    if ((UART0_FR_R & UART_FR_RXFE) != 0u){
        return -1;
    }
    return (int32_t)(UART0_DR_R & 0xFFu);
}
