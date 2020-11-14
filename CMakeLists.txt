cmake_minimum_required(VERSION 3.17)

project(cnc CXX)

include_directories(/usr/avr/include)
include_directories(/home/kir/projects/AVRC/include)

set(SOURCES main.c avr.h cnc.h gcode.h firmware.c avr/eeprom.c avr/eeprom.h avr/uart.h avr/uart.c avr/config.h avr/cnc.h avr/cnc.c avr/gcode.c avr/gcode.h)

add_executable(cnc ${SOURCES})