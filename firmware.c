#include "avr/uart.h"
#include "avr/cnc.h"

void send();

int main()
{
    uart_init();
//    cnc_init();

    sei();

    cnc_main_thread();
}