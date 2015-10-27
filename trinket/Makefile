all : 24AA.elf

test : main.hex

main.hex : main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=attiny85 main.elf
	
24AA.elf : 24AA.c
	avr-gcc -Os -DF_CPU=8000000 -mmcu=attiny85 -c 24AA.c -o 24AA.elf
	
main.elf : main.c 24AA.elf
	avr-gcc -Os -DF_CPU=8000000 -mmcu=attiny85 main.c 24AA.elf -o main.elf
	
prog : main.hex
	avrdude -c usbtiny -p attiny85 -U flash:w:main.hex:i