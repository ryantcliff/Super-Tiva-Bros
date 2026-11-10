#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_write_char(char character);
void uart_write_string(const char *text);
int32_t uart_read_char(void);

#endif
