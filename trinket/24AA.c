#include "24AA.h"

#define AA_CONTROL 0xA0
#define AA_WRITE 0x0
#define AA_READ 0x1

int writeByte(char chipSelect, char addrH, char addrL, char byte)
{
	return writePage(chipSelect, addrH, addrL, &byte, 1);
}

int ackPoll(char chipSelect)
{
	if (chipSelect > 7) // three bits only
	{
		return 0;
	}
	//Acknowledge polling
	do {
		//Lets try talking to a 24AA
		i2c_master_start();
	
		i2c_master_writeByte(AA_CONTROL | chipSelect | AA_WRITE); //Write
		
		//If the devices doesn't return an ACK, then it is probably busy, keep trying. 
		//TODO: It's possible that if the device dies/disconnects, whatever we wont' ever receive an ack.
		//Time out after some amount of time. 
	} while (i2c_master_readNAck());
		
	return 1;
}

int readSequential(char chipSelect, char addrH, char addrL, char bytes[], int count)
{
	if (!ackPoll(chipSelect))
	{
		return 0;
	}
	i2c_master_start();
	
	i2c_master_writeByte(AA_CONTROL | chipSelect | AA_READ); //Read
	i2c_master_readNAck();
	
	int i;
	for (i = 0; i < count-1; ++i)
	{
		bytes[i] = i2c_master_readByte();
		i2c_master_writeAck();
		
	}
	bytes[count-1] = i2c_master_readByte();
	i2c_master_writeNack();
	
	i2c_master_stop();
	
	return 1;
	
}

int readRandom(char chipSelect, char addrH, char addrL, char * byte)
{
	if (!ackPoll(chipSelect))
	{
		return 0;
	}
	
	return readCurrent(chipSelect, byte);
}

int readCurrent(char chipSelect, char * byte)
{
	if (chipSelect > 7) // three bits only
	{
		return 0;
	}
	
	i2c_master_start();
	
	i2c_master_writeByte(AA_CONTROL | chipSelect | AA_READ); //Read
	i2c_master_readNAck();
	*byte = i2c_master_readByte();
	i2c_master_writeNack();
	
	i2c_master_stop();
	
	return 1;
}

int writePage(char chipSelect, char addrH, char addrL, char bytes[], int count)
{
	//Poll the device until it has finished it's write cycle
	//if time out, fail
	if(count > 64 || !ackPoll(chipSelect))
	{
		return 0;
	}
		
	i2c_master_writeByte(addrH); //Address High Byte
	i2c_master_readNAck();
	i2c_master_writeByte(addrL); //Address Low Byte
	i2c_master_readNAck();
	int i;
	for (i = 0; i < count; ++i)
	{
		i2c_master_writeByte(bytes[i]); 
		if (i2c_master_readNAck()) //We got a nack
		{
			break;
		}
	}
	i2c_master_stop();
	
	return 1;
}
