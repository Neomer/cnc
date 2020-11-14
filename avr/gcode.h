//
// Created by kir on 14.11.2020.
//

#ifndef CNC_GCODE_H
#define CNC_GCODE_H

#include <inttypes.h>

#define GCODE_TERMINATE_CODE              0
#define GCODE_COMMAND_SET_COUNT           10

enum gcode_value_types {
    GCODE_VALUE_INT,
    GCODE_VALUE_FLOAT,
};

struct gcode_value {
    union {
        uint16_t vInt;
        float vFloat;
    } value;
    enum gcode_value_types type;
};

struct gcode_command {
    char type;
    struct gcode_value value;
};

struct gcode_command_set {
    uint8_t count;
    struct gcode_command commandList[GCODE_COMMAND_SET_COUNT];
};

void gcode_exec(const char *gcode, uint8_t size);

#endif //CNC_GCODE_H
