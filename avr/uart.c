//
// Created by kir on 13.11.2020.
//

#include "uart.h"
#include "gcode.h"

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#define UART_RX_BUFFER_SIZE            128
#define UART_TX_BUFFER_SIZE            30

static volatile char rx[UART_RX_BUFFER_SIZE + 1];
static volatile char tx[UART_TX_BUFFER_SIZE + 1];
static volatile uint8_t wp = 0;
static volatile uint8_t rp = 0;

inline void uart_en_rx() {
    UCSRB &= ~(1 << UDRIE);
    UCSRB |= (1 << RXEN);
}


inline void uart_en_tx() {
    UCSRB |= (1 << UDRIE);
    UCSRB &= ~(1 << RXEN);
}

/**
 * Called when USART completes receiving data
 *
 * It checks if there's no error, and if the character r is received,
 * and ADC conversion is started
 */
ISR(USART_RXC_vect)
{
    char ch = UDR;
    if ((UCSRA & UART_ERROR_MASK) == 0) {
        if (ch == '_' || wp >= UART_RX_BUFFER_SIZE) {
            rx[wp] = 0;
            gcode_exec((const char *)rx, wp);
        } else {
            rx[wp++] = ch;
        }
    }
}

/**
 * Called when the data register accepts new data
 *
 * When this occurs, we can write new data through the USART,
 * and in this case we write the ADCH value.
 */
ISR(USART_UDRE_vect)
{
    if (tx[rp] == 0) {
        rp = 0;
        wp = 0;

        uart_en_rx();
    } else {
        UDR = tx[rp++];
    }
}

void uart_init()
{
    // setup baud-rate
    UBRRH = UBRR_H;
    UBRRL = UBRR_L;

    // setup params
    UCSRB = CSRCB;
    UCSRC = CSRCC;
}

void uart_write(const char *data)
{
    uint8_t i;
    for (i = 0; i < UART_TX_BUFFER_SIZE && data[i]; ++i) {
        tx[i] = data[i];
    }
    uart_en_tx();
}
