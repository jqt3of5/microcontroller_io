all : 24AA.elf i2c.elf 

test : main.hex

clean :
	rm main.hex main.elf 24AA.elf i2c.elf 

main.hex : main.c 24AA.elf i2c.elf
	avr-gcc -Os -DF_CPU=8000000 -mmcu=attiny85 main.c 24AA.elf i2c.elf -o main.elf
	avr-size --format=avr --mcu=attiny85 main.elf	
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	
i2c.elf : i2c.c
	avr-gcc -Os -DF_CPU=8000000 -mmcu=attiny85 -c i2c.c -o i2c.elf
	avr-size --format=avr --mcu=attiny85 i2c.elf
24AA.elf : 24AA.c
	avr-gcc -Os -DF_CPU=8000000 -mmcu=attiny85 -c 24AA.c -o 24AA.elf
	avr-size --format=avr --mcu=attiny85 24AA.elf
prog : main.hex
	avrdude -c usbtiny -p attiny85 -U flash:w:main.hex:i