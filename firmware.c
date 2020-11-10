#define F_CPU 8000000UL

#include "AVRC/include/interface/uart.h"
#include "gcode.h"
#include <util/delay.h>

int main()
{
    uart_init(UART_BAUD_8MHz_57600);

    char buf[125];
    struct cnc_context ctx;
    while (1) {
        uint8_t len = uart_read_array((uint8_t *)buf, 125, '\n');
        buf[len] = 0;
        if (len) {
            gcode_exec(buf, len, &ctx);
            uart_send_str("OK");
        }
    }
}
