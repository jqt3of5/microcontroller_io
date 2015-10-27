#include "i2c.h"
#include "LSM303.h"

#define LSM_ACC_ADDR 0b00110010
#define LSM_MAG_ADDR 0b00111100 
#define READ 1
#define WRITE 0

void LSM303_write_byte(char device, char addr,char byte)
{
	i2c_master_start();
	i2c_master_writeByte(device | WRITE);
	i2c_master_readNAck();
	
	i2c_master_writeByte(addr);
	i2c_master_readNAck();
	
	i2c_master_writeByte(byte);
	i2c_master_readNAck();
	
	i2c_master_stop();
}

void LSM303_read_bytes(char device, char addr, char bytes[], char count)
{
	i2c_master_start();
	i2c_master_writeByte(device | WRITE); 
	i2c_master_readNAck();
	i2c_master_writeByte(addr);
	i2c_master_readNAck();
	
	i2c_master_start();
	i2c_master_writeByte(device | READ); 
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
}

#define OUT_X_L_A 0x28
#define OUT_Y_L_A 0x2A
#define OUT_Z_L_A 0x2C
#define ACC_START OUT_X_L_A

//Needs an array at least 6 bytes in size
void LSM303_ACC_readData(char bytes[])
{
	LSM303_read_bytes(LSM_ACC_ADDR, ACC_START, bytes, 6);
}
#define OUT_X_H_M 0x03
#define OUT_Y_H_M 0x05
#define OUT_Z_H_M 0x07
#define MAG_START OUT_X_H_M

//Needs an array at least 6 bytes in size
void LSM303_MAG_readMagData(char bytes[])
{
	LSM303_read_bytes(LSM_MAG_ADDR, MAG_START, bytes, 6);
}

#define TEMP_OUT_H_M 0x31
#define TEMP_OUT_L_M 0x32
#define TEMP_START 0x31
//Needs an array at least 2 bytes in size
void LSM303_MAG_readTempData(char bytes[])
{
	LSM303_read_bytes(LSM_MAG_ADDR, TEMP_START, bytes, 2);
}

#define STATUS_REG_ACC 0x27
#define XYZ_OVERRUN 7
#define Z_OVERRUN 6
#define Y_OVERRUN 5
#define X_OVERRUN 4
#define XYZ_NEW_DATA 3
#define Z_NEW_DATA 2
#define Y_NEW_DATA 1
#define X_NEW_DATA 0

#define CRA_REG_MAG 0x00
#define TEMP_EN 7
#define DO0 2
#define CRB_REG_MAG 0x00
#define GN0 5

void LSM303_MAG_configure(int tempEnable, int dataRate, int gain)
{
	LSM303_write_byte(LSM_MAG_ADDR, CRA_REG_MAG, (tempEnable << TEMP_EN) | (dataRate << DO0));
	LSM303_write_byte(LSM_MAG_ADDR, CRB_REG_MAG, (gain << GN0));
}

#define CONTROL_REG1_ACC 0x20
#define ODR0 4
#define Z_ENABLE 2
#define Y_ENABLE 1
#define X_ENABLE 0

void LSM303_ACC_powerup(int sampleRate, int x, int y, int z)
{
	LSM303_write_byte(LSM_ACC_ADDR, CONTROL_REG1_ACC, (sampleRate << ODR0) | (z << Z_ENABLE) | (y << Y_ENABLE) | (z << Z_ENABLE));
}

#define CONTROL_REG4_ACC 0x23
#define BDU 7
#define BLE 6
#define FS0 4
#define HR 3
#define SIM 1
void LSM303_ACC_configure(int continuous, int bigLittleEndian, int fullScale, int highRes, int spiMode)
{
	LSM303_write_byte(LSM_ACC_ADDR, CONTROL_REG4_ACC, (continuous << BDU) | (bigLittleEndian << BLE) | (fullScale << FS0) | (highRes << HR) | (spiMode << SIM));
}