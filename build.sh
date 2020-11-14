avr-g++ -Wall -g -Os -mmcu=atmega8 -o ./build/firmware.elf firmware.c ./avr/uart.c ./avr/cnc.c ./avr/eeprom.c ./avr/gcode.c
avr-size -C ./build/firmware.elf
avr-objcopy -j .text -j .data -O ihex ./build/firmware.elf ./build/firmware.hex