#ifndef __RPI_UART_H
#define __RPI_UART_H

#pragma once

void rpi_putchar(char ch);

void uart_init(void);
void uart_write_text(const char *buffer);

#endif

