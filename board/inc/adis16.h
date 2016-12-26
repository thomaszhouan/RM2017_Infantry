#ifndef ADIS16_H
#define ADIS16_H

#ifndef ADIS16_FILE
    #define ADIS16_EXT extern
#else
    #define ADIS16_EXT
#endif

typedef struct {
    int16_t omega;
    float theta;
    uint32_t thetaHardware;
    uint32_t lastUpdateTick;
} ADIS16_DataTypeDef;

ADIS16_EXT volatile ADIS16_DataTypeDef ADIS16_Data;

void ADIS16_Init(void);
void ADIS16_Update(void);
void ADIS16_Calibrate(uint16_t sample);

#endif // ADIS16_H
