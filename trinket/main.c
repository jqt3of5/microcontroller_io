#include "24AA.h"
#include <avr/io.h>
#include <util/delay.h>

void blinkCount(char count)
{
	PORTB &= ~(1 << PB1);  
	_delay_ms(800);
	while (count > 0)
	{
		PORTB |= 1 << PB1; 
		_delay_ms(400);
		PORTB &= ~(1 << PB1); 
		_delay_ms(400);
		count--;
	}
}

int main()
{
	// set PB3 to be output
	DDRB = (1<<PB1); /*for the LED*/
	
	i2c_master_init();
	
	//allow some setup time
	_delay_ms(40);
	
	if (!writeByte(0x0, 0x1, 0x1, 0x25))
	{
		while(1) blinkCount(1);
	}
	
	char byte = 0;
	if (!readRandom(0x0, 0x1, 0x1, &byte))
	{
		while(1) blinkCount(1);
	}
	
	if (byte != 0x25)
	{
		while (1) blinkCount(1);
	}
	
	while (1) blinkCount(2);
	return 1;
}

