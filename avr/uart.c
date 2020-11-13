//
// Created by kir on 13.11.2020.
//

#include "uart.h"

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

#define UART_RX_BUFFER_SIZE            128
#define UART_TX_BUFFER_SIZE            10

volatile char rx[UART_RX_BUFFER_SIZE];
volatile const char *tx = "ok";
volatile uint8_t wp = 0;
volatile uint8_t rp = 0;

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
            UCSRB &= ~(1 << RXEN);
            UCSRB |= (1 << UDRIE);
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
    if (rp >= 3) {
        rp = 0;
        wp = 0;

        UCSRB &= ~(1 << UDRIE);
        UCSRB |= (1 << RXEN);
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
