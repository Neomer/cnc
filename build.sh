avr-g++ -Wall -g -Os -mmcu=atmega8 -o ./build/firmware.elf firmware.c
avr-size -C ./build/firmware.elf
avr-objcopy -j .text -j .data -O ihex ./build/firmware.elf ./build/firmware.hex