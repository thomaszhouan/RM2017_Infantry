#ifndef MPU6050_H
#define MPU6050_H

#ifndef MPU6050_FILE
    #define MPU6050_EXT extern
#else
    #define MPU6050_EXT
#endif // MPU6050_FILE

typedef enum {
    kAFS_2G,
    kAFS_4G,
    kAFS_8G,
    kAFS_16G
} MPU6050_AccelScale;

typedef enum {
    kGFS_250DPS,
    kGFS_500DPS,
    kGFS_1000DPS,
    kGFS_2000DPS
} MPU6050_GyroScale;

typedef struct {
    MPU6050_AccelScale afs;
    MPU6050_GyroScale gfs;
} MPU6050_ConfigTypeDef;

MPU6050_EXT volatile int16_t MPU6050_GyroData[3];
MPU6050_EXT volatile int16_t MPU6050_AccelData[3];

void MPU6050_Init(void);
void MPU6050_Config(MPU6050_ConfigTypeDef *config);
void MPU6050_ReadGyroData(void);
void MPU6050_ReadAccelData(void);

#endif // MPU6050_H
