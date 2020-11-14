//
// Created by kir on 13.11.2020.
//

#include "cnc.h"
#include "uart.h"

void cnc_init()
{
    CNC_AXIS_DDR |= (1 << CNC_AXIS_X_CLOCK_PIN) | (1 << CNC_AXIS_Y_CLOCK_PIN) | (1 << CNC_AXIS_Z_CLOCK_PIN);
    CNC_AXIS_PORT = 0x00;

    cnc_load_context();
    cnc_find_home();
}

void cnc_load_context()
{
    cnc_stop_movement();
}

#include <util/delay.h>

void cnc_main_thread()
{
    volatile uint8_t old_movement_mask = 0x00;
    while (1) {

        volatile uint8_t movement_mask = 0x00;

        if (cnc_context.movement.x.steps > 0) {
            if (!old_movement_mask) {
                uart_write("move x");
            }
            movement_mask |= (1u << CNC_AXIS_X_CLOCK_PIN);
            --cnc_context.movement.x.steps;
        }
        if (cnc_context.movement.y.steps > 0) {
            movement_mask |= (1u << CNC_AXIS_Y_CLOCK_PIN);
            --cnc_context.movement.y.steps;
        }
        if (cnc_context.movement.z.steps > 0) {
            movement_mask |= (1u << CNC_AXIS_Z_CLOCK_PIN);
            --cnc_context.movement.z.steps;
        }

        CNC_AXIS_PORT |= movement_mask;
        _delay_ms(1);
        CNC_AXIS_PORT &= ~movement_mask;
        _delay_ms(1);

        if (movement_mask == 0 && old_movement_mask != 0) {
            uart_write("movement done");
        }
        old_movement_mask = movement_mask;
    }
}
