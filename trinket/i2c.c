#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>
#define SDA (1<<PB0)
#define SCL (1<<PB2)

#define I2C_DELAY 5
void i2c_master_init()
{
	DDRB |= SDA | SCL; //SDA/SCL output	
	PORTB |= SDA | SCL; //SDA/SCL pull up	
	USICR = (1<<USIWM1) //two wire mode
			| (1<<USICS1) | (1<<USICLK)
			| (0<<USITC); //Clock source is software strobe USITC, positive edge
  
	USIDR = 0xFF;
	USISR   =   (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Clear flags,
              (0x0<<USICNT0);    
}
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
		_delay_ms(I2C_DELAY);
		USICR = byte; //Toggle clock high
		while (!(PORTB & SCL));
		_delay_ms(I2C_DELAY);
		USICR = byte; // Toggle clock low
	} while (!(USISR & (1<< USIOIF)));
	
	_delay_ms(I2C_DELAY);
	byte = USIDR;
	USIDR = 0xFF;
	DDRB |= SDA;
	
	return byte;
}

void i2c_master_writeByte(char byte)
{
	transfer(byte, 8, 0);
}
char i2c_master_readByte()
{
	return transfer(0,8, 1);
}
void i2c_master_writeNack()
{
	transfer(0xFF, 1, 0);
}
void i2c_master_writeAck()
{
	transfer(0, 1, 0);
}
char i2c_master_readNAck()
{
	return transfer(0, 1, 1);
}

void i2c_master_start()
{
	PORTB |= SCL; //release clock
	while (!(PORTB & SCL)); // SCL has to become high
	_delay_ms(I2C_DELAY);
	
	PORTB &= ~SDA;
	_delay_ms(I2C_DELAY);
	
	PORTB &= ~SCL;
	PORTB |= SDA; //Release data
}

void i2c_master_stop()
{
	PORTB &= ~SDA;
	PORTB |= SCL;
	while (!(PORTB | SCL));
	_delay_ms(I2C_DELAY);
	
	PORTB |= SDA;
	_delay_ms(I2C_DELAY);
}

