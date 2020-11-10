//
// Created by kir on 08.11.2020.
//

#ifndef CNC_GCODE_H
#define CNC_GCODE_H

#include <inttypes.h>
#include <string.h>
#include "cnc.h"

#ifdef FIRMWARE
#include <avr/io.h>
#include <util/delay.h>
#endif

#ifndef AXIS_X_CLOCK_PIN
#define AXIS_X_CLOCK_PIN            PB0
#endif

#ifndef AXIS_X_DIRECTION_PIN
#define AXIS_X_DIRECTION_PIN        PB1
#endif

#ifndef AXIS_Y_CLOCK_PIN
#define AXIS_Y_CLOCK_PIN            PB2
#endif

#ifndef AXIS_Y_DIRECTION_PIN
#define AXIS_Y_DIRECTION_PIN        PB3
#endif

#ifndef AXIS_Z_CLOCK_PIN
#define AXIS_Z_CLOCK_PIN            PB4
#endif

#ifndef AXIS_Z_DIRECTION_PIN
#define AXIS_Z_DIRECTION_PIN        PB5
#endif

#define TERMINATE_CODE              0
#define COMMAND_SET_COUNT           10
#define DELAY_STEP                  10

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
    struct gcode_command commandList[COMMAND_SET_COUNT];
};

struct gcode_command *gcode_get_command_from_set(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx);

uint8_t gcode_read_command(char *data, const char *end, struct gcode_command *command);

void gcode_process_g(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx, struct cnc_context *pContext);

void gcode_action_move(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx, struct cnc_context *pContext);

void gcode_action_reset_origin(struct gcode_command_set *pSet, uint8_t *idx, struct cnc_context *pContext);

void gcode_action_delay(struct gcode_command_set *pSet, uint8_t *idx);

void gcode_exec(char *data, uint8_t size, struct cnc_context *ctx) {
    char *ptr = data;
    const char *end = data + size;

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
    printf("parsing done. %d commands total.\n", cmd.count);

    for (uint8_t reader = 0; reader < cmd.count; ++reader) {
        switch (cmd.commandList[reader].type) {
            case 'G': gcode_process_g(&cmd, &reader, ctx);
        }
    }
}

char gcode_is_separator(char sym) {
    return sym == '\n' || sym == ' ' || sym == 0;
}

uint8_t gcode_read_command(char *data, const char *end, struct gcode_command *command) {
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

struct gcode_command *gcode_get_command_from_set(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx)
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

void gcode_process_g(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx, struct cnc_context *pContext)
{
    struct gcode_command *pCommand = gcode_get_command_from_set(pCommandsSet, pReaderIdx);

    switch (pCommand->value.value.vInt) {
        case 0: {
            gcode_action_move(pCommandsSet, pReaderIdx, pContext);
            break;
        }

        case 1: {

        }

        case 2: {

        }

        case 3: {

        }

        case 4: {
            gcode_action_delay(pCommandsSet, pReaderIdx);
        }

        case 10: {
            gcode_action_reset_origin(pCommandsSet, pReaderIdx, pContext);
        }
    }
}

void gcode_action_delay(struct gcode_command_set *pSet, uint8_t *idx)
{
    struct gcode_command *pCommand = gcode_get_command_from_set(pSet, idx);
    if (pCommand == 0) {
        printf("unexpected end of command.\n");
        return;
    }
    int32_t ms;
    switch (pCommand->type) {
        case 'X': {
            if (pCommand->value.type == GCODE_VALUE_INT) {
                ms = pCommand->value.value.vInt * 1000;
            } else {
                ms = (int32_t)(pCommand->value.value.vFloat * 1000);
            }
            break;
        }
        case 'P': {
            if (pCommand->value.type == GCODE_VALUE_INT) {
                ms = pCommand->value.value.vInt;
            } else {
                ms = (int32_t)(pCommand->value.value.vFloat);
            }
            break;
        }
        default: {
            printf("unexpected command. C: %c\n", pCommand->type);
            return;
        }
    }

    printf("delay execution on %d ms\n", ms);
    while ((ms -= DELAY_STEP) > 0) {
//        _delay_ms(DELAY_STEP);
    }
}

void gcode_action_reset_origin(struct gcode_command_set *pSet, uint8_t *idx, struct cnc_context *pContext)
{

}

void gcode_action_move(struct gcode_command_set *pCommandsSet, uint8_t *pReaderIdx, struct cnc_context *pContext)
{
    struct axis ax;
    memset(&ax, 0, sizeof(struct axis));
    struct gcode_command *pCommand;
    while ((pCommand = gcode_get_command_from_set(pCommandsSet, pReaderIdx)) != 0) {
        switch (pCommand->type) {
            case 'X': ax.x = pCommand->value.type == GCODE_VALUE_INT ? (float)pCommand->value.value.vInt : pCommand->value.value.vFloat; break;
            case 'Y': ax.y = pCommand->value.type == GCODE_VALUE_INT ? (float)pCommand->value.value.vInt : pCommand->value.value.vFloat; break;
            case 'Z': ax.z = pCommand->value.type == GCODE_VALUE_INT ? (float)pCommand->value.value.vInt : pCommand->value.value.vFloat; break;
            default: {
                printf("unexpected command. C: %c\n", pCommand->type);
                return;
            }
        }
    }

    printf("moving to pos (%f, %f, %f)\n", ax.x, ax.y, ax.z);

    
}


#endif //CNC_GCODE_H
