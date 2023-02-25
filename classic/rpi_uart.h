#ifndef __RPI_UART_H
#define __RPI_UART_H

#pragma once

void uart_init(void);

void uart_send(unsigned char ch);

void uart_hex(unsigned int hex);

unsigned char uart_getc(void);

void uart_puts(const char *buffer);


#endif
