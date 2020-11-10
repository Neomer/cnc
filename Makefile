build:
	avr-g++ -Wall -g -Os -mmcu=atmega8 -o ./build/main.elf main.c
	avr-size -C ./build/main.elf
	avr-objcopy -j .text -j .data -O ihex ./build/main.elf ./build/main.hex