#include "i2c.h"

int writeByte(char chipSelect, char addrH, char addrL, char byte);
int writePage(char chipSelect, char addrH, char addrL, char bytes[], int count);

int readSequential(char chipSelect, char addrH, char addrL, char bytes[], int count);
int readRandom(char chipSelect, char addrH, char addrL, char * byte);
int readCurrent(char chipSelect, char * byte);

