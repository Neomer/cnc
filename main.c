//
// Created by kir on 08.11.2020.
//
#include <stdio.h>
#include <string.h>
#include "gcode.h"

int main() {
    char *data = "G04 P700\n";

    struct cnc_context ctx;
    ctx.axis_position.x = 0;
    ctx.axis_position.y = 0;
    ctx.axis_position.z = 0;

    gcode_exec(data, strlen(data), &ctx);

    return 0;
}