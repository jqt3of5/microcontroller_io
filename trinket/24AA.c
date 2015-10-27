#include "24AA.h"
#include <avr/io.h>
#include <util/delay.h>
#define SDA (1<<PB0)
#define SCL (1<<PB2)
#define AA_CONTROL 0xA0
#define AA_WRITE 0x0;
#define AA_READ 0x1;

char transfer(char byte, int count, char isInput)
{
	unsigned char const clear_USISR = (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Prepare register value to: Clear flags, and
                                     (0x0<<USICNT0); //Shifting 8 bits, 16 clock edges
	//DDRB need to set in/out. Right?
	if (isInput != 0)
	{
		DDRB &= ~(1<<SDA);
	}
	else
	{
		DDRB |= 1<<SDA;
	}
	//count must be less than 16
	USISR = clear_USISR | ((0x10-count*2)<<USICNT0);//Set the bit transfer count *2 for clock edges
	USIDR = byte; 
	
	byte =  (1<<USIWM1) //two wire mode
			| (1<<USICS1) | (1<<USICLK)
			| (1<<USITC); //Toggle clock
			
	do {
		_delay_ms(20);
		USICR = byte; //Toggle clock high
		while (!(PORTB & SCL));
		_delay_ms(20);
		USICR = byte; // Toggle clock low
	} while (!(USISR & (1<< USIOIF)));
	
	_delay_ms(20);
	byte = USIDR;
	USIDR = 0xFF;
	DDRB |= SDA;
	
	return byte;
}

void writeByte(char byte)
{
	transfer(byte, 8, 0);
}
char readByte()
{
	return transfer(0,8, 1);
}
void writeNack()
{
	transfer(0xFF, 1, 0);
}
void writeAck()
{
	transfer(0, 1, 0);
}
char readNAck()
{
	return transfer(0, 1, 1);
}

void start()
{
	PORTB |= SCL; //release clock
	while (!(PORTB & SCL)); // SCL has to become high
	_delay_ms(20);
	
	PORTB &= ~SDA;
	_delay_ms(20);
	
	PORTB &= ~SCL;
	PORTB |= SDA; //Release data
}

void stop()
{
	PORTB &= ~SDA;
	PORTB |= SCL;
	while (!(PORTB | SCL));
	_delay_ms(20);
	
	PORTB |= SDA;
	_delay_ms(20);
}


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

void testReadWrite (void)
{
	// set PB3 to be output
	DDRB = (1<<PB1) /*for the LED*/
			| SDA | SCL; //SDA/SCL output	
	PORTB = SDA | SCL; //SDA/SCL pull up	
	USICR = (1<<USIWM1) //two wire mode
			| (1<<USICS1) | (1<<USICLK)
			| (0<<USITC); //Clock source is software strobe USITC, positive edge
	
  
	USIDR = 0xFF;
	USISR   =   (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Clear flags,
              (0x0<<USICNT0);    
  
	//Lets try talking to a 24AA
	start();
	writeByte(AA_CONTROL | 0x0 | AA_WRITE) //Write
	readNAck();
	writeByte(0x0); //Address High Byte
	readNAck();
	writeByte(0x0); //Address Low Byte
	readNACK();
	writeByte(0x25); 
	readNAck();
	stop();
	
	start();
	writeByte(AA_CONTROL | 0x0 | AA_WRITE) //Random Read
	readNAck();
	writeByte(0x0); //Address High Byte
	readNAck();
	writeByte(0x0); //Address Low Byte
	readNACK();
	start();
	writeByte(AA_CONTROL | 0x0 | AA_READ) //Read
	char byte = readByte();
	readNAck();
	stop();
 
	if (byte == 0x25)
	{
		while(1)
		{
			blinkCount(1);
		}
	}
	else
	{
		while(1)
		{
			blinkCount(byte >> 4);
			blinkCount(byte & 0xF);
		}
	}
}