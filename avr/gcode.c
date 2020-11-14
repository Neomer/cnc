//
// Created by kir on 14.11.2020.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gcode.h"
#include "cnc.h"
#include "uart.h"

#define TERMINATE_CODE              0
#define COMMAND_SET_COUNT           10

inline char gcode_is_separator(char sym);

inline uint8_t gcode_read_command(char *data, const char *end, struct gcode_command *command);

inline struct gcode_command *gcode_get_command_from_set(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx);

inline void gcode_process_g(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx);

inline void gcode_action_move(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx);

void gcode_exec(const char *gcode, uint8_t size)
{
    char *ptr = (char*)gcode;
    const char *end = gcode + size;

    struct gcode_command_set cmd;
    cmd.count = 0;

    while (ptr != end) {
        uint8_t len = gcode_read_command(ptr, end, cmd.commandList + cmd.count);
        if (!len) {
            break;
        }
        ptr += len;
        ++cmd.count;
    }
    uart_write("gcode ok");

    for (uint8_t reader = 0; reader < cmd.count; ++reader) {
        switch (cmd.commandList[reader].type) {
            case 'G': gcode_process_g(&cmd, &reader);
        }
    }
}

inline char gcode_is_separator(char sym) {
    return sym == '\n' || sym == ' ' || sym == 0;
}

inline uint8_t gcode_read_command(char *data, const char *end, struct gcode_command *command) {
    char *ptr = data;

    while (ptr != end && gcode_is_separator(*ptr)) {
        ++ptr;
    }
    if (ptr == end) {
        command->type = TERMINATE_CODE;
        return 0;
    }
    command->type = *ptr++;

    int8_t multiply;
    if (*ptr == '-') {
        multiply = -1;
        ++ptr;
    } else {
        multiply = 1;
    }
    uint16_t tmp = 0;
    float tmpFloat = 0;
    float float_multiply = 0.1f;
    enum gcode_value_types type = GCODE_VALUE_INT;
    while (ptr != end && !gcode_is_separator(*ptr)) {
        if (*ptr == '.') {
            type = GCODE_VALUE_FLOAT;
            tmpFloat = tmp;
        } else if (type == GCODE_VALUE_INT) {
            tmp = tmp * 10 + (*ptr - '0');
        } else {
            // todo: 600 bytes
            tmpFloat = tmpFloat + ((float)(*ptr - '0') * float_multiply);
            float_multiply *= 0.1f;
        }
        ++ptr;
    }
    command->value.type = type;
    if (type == GCODE_VALUE_INT) {
        command->value.value.vInt = multiply * tmp;
    } else {
        command->value.value.vFloat = (float)multiply * tmpFloat;
    }

    return (uint8_t)(ptr - data);
}

inline struct gcode_command *gcode_get_command_from_set(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx)
{
    if (*pReaderIdx == pCommandsSet->count) {
        return 0;
    }
    uint8_t idx = *pReaderIdx;
    ++*pReaderIdx;
    return &pCommandsSet->commandList[idx];
}

/*****************************
 *
 * ACTIONS
 *
 */

inline void gcode_process_g(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx)
{
    struct gcode_command *pCommand = gcode_get_command_from_set(pCommandsSet, pReaderIdx);

    switch (pCommand->value.value.vInt) {
        case 0: {
            cnc_stop_movement();
            break;
        }

        case 1: {
            gcode_action_move(pCommandsSet, pReaderIdx);
            break;
        }

        case 2: {

        }

        case 3: {

        }

        case 4: {
        }

        case 10: {
        }
    }
}


inline void gcode_action_move(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx)
{
    cnc_stop_movement();

    struct gcode_command *pCommand;
    while ((pCommand = gcode_get_command_from_set(pCommandsSet, pReaderIdx)) != 0) {
        switch (pCommand->type) {
            case 'X': cnc_context.movement.x.steps = pCommand->value.value.vInt; break;
            case 'Y': cnc_context.movement.y.steps = pCommand->value.value.vInt; break;
            case 'Z': cnc_context.movement.z.steps = pCommand->value.value.vInt; break;
            default: {
                return;
            }
        }
    }
}