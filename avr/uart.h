//
// Created by kir on 13.11.2020.
//

#ifndef CNC_UART_H
#define CNC_UART_H

#include <avr/interrupt.h>
#include "config.h"

#define UBRR_H      0x00
#define UBRR_L      0x08

#define CSRCB       0b11011000
#define CSRCC       0b10000110

#define UART_ERROR_MASK     0b00011100

void uart_init();

void uart_write(const char *data);

#endif //CNC_UART_H
