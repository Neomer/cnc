//
// Created by kir on 08.11.2020.
//

#ifndef CNC_AVR_H
#define CNC_AVR_H

#include <avr/io.h>

enum Direction {
    READ,
    WRITE
};

void pinMode(unsigned char p, enum Direction d) {
    DDRD |= d << p;
}

#endif //CNC_AVR_H
