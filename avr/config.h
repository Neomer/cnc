//
// Created by kir on 13.11.2020.
//

#ifndef CNC_CONFIG_H
#define CNC_CONFIG_H

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 8000000
#endif

#define CNC_AXIS_PORT               PORTC
#define CNC_AXIS_DDR                DDRC

#define CNC_AXIS_X_CLOCK_PIN        1u
#define CNC_AXIS_Y_CLOCK_PIN        3u
#define CNC_AXIS_Z_CLOCK_PIN        5u

#define CNC_STEP_DELAY              1u

#endif //CNC_CONFIG_H
