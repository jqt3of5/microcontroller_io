#define LSM303_MAG_DO_75 0
#define LSM303_MAG_DO_150 1
#define LSM303_MAG_DO_300 2
#define LSM303_MAG_DO_750 3
#define LSM303_MAG_DO_1500 4
#define LSM303_MAG_DO_3000 5
#define LSM303_MAG_DO_7500 6
#define LSM303_MAG_DO_22000 7

#define LSM303_MAG_GN_13 1
#define LSM303_MAG_GN_19 2
#define LSM303_MAG_GN_25 3
#define LSM303_MAG_GN_40 4
#define LSM303_MAG_GN_47 5
#define LSM303_MAG_GN_56 6
#define LSM303_MAG_GN_81 7

void LSM303_MAG_configure(int tempEnable, int dataRate, int gain);

#define LSM303_ACC_ODR_PWR_DWN 0x0
#define LSM303_ACC_ODR_1 0x1
#define LSM303_ACC_ODR_10 0x2
#define LSM303_ACC_ODR_25 0x3
#define LSM303_ACC_ODR_50 0x4
#define LSM303_ACC_ODR_100 0x5
#define LSM303_ACC_ODR_200 0x6
#define LSM303_ACC_ODR_400 0x7

void LSM303_ACC_powerup(int sampleRate, int x, int y, int z);

#define LSM303_ACC_FS_2 0
#define LSM303_ACC_FS_4 1
#define LSM303_ACC_FS_8 2
#define LSM303_ACC_FS_16 3
void LSM303_ACC_configure(int continuous, int bigLittleEndian, int fullScale, int highRes, int spiMode);

void LSM303_ACC_readData(char bytes[]);
void LSM303_MAG_readMagData(char bytes[]);
void LSM303_MAG_readTempData(char bytes[]);
