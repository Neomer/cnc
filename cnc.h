//
// Created by kir on 08.11.2020.
//

#ifndef CNC_CNC_H
#define CNC_CNC_H

#include <inttypes.h>

struct axis {
    float x;
    float y;
    float z;
};

struct cnc_props {
    uint16_t spindle_speed; // Скорость шпинделя (об/мин)
    uint16_t drilling_speed;  // Скорость рабочей подачи (мм/мин)
    struct axis moving_per_step; // Перемещение за 1 шаг двигателя
};

struct cnc_context {
    struct cnc_props props;
    struct axis axis_position;
};

#endif //CNC_CNC_H
