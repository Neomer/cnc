//
// Created by kir on 13.11.2020.
//

#ifndef CNC_CNC_H
#define CNC_CNC_H

#include "config.h"

enum e_cnc_axis_direction {
    DIRECTION_NONE = -1,
    DIRECTION_FORWARD = 0,
    DIRECTION_BACWARD = 1
};

struct s_cnc_axis_movement {
    volatile enum e_cnc_axis_direction direction;
    volatile int steps;
};

struct s_cnc_movement {
    volatile struct s_cnc_axis_movement x;
    volatile struct s_cnc_axis_movement y;
    volatile struct s_cnc_axis_movement z;
};

struct s_cnc_context {
    volatile struct s_cnc_movement movement;
};

static struct s_cnc_context cnc_context;

void cnc_init();

void cnc_load_context();

inline void cnc_find_home()
{
}

inline void cnc_stop_movement()
{
    cnc_context.movement.x.steps = 0;
    cnc_context.movement.y.steps = 0;
    cnc_context.movement.z.steps = 0;
}

void cnc_main_thread();


#endif //CNC_CNC_H
