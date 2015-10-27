//======================================================================================
// Name: i2c-master.h
//
// Description: This file provides the interface for the i2c master driver designed and tested on
// a trinket (attiny85)
//
//======================================================================================

void i2c_master_init();

// Signal i2c start condition
void i2c_master_start();
// Signal i2c stop condition
void i2c_master_stop();

//write a single byte to the buffer
void i2c_master_writeByte(char byte);
//read a single byte from the buffer
char i2c_master_readByte();
//write a Nack
void i2c_master_writeNack();
//write an ack
void i2c_master_writeAck();
//read either an ack or a nack, and return the result. 
char i2c_master_readNAck();