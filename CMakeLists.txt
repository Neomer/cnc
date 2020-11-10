cmake_minimum_required(VERSION 3.17)

project(cnc C)

include_directories(/usr/lib/avr/include)

set(SOURCES main.c avr.h cnc.h gcode.h)

add_executable(cnc ${SOURCES})