#ifndef ADIS16_H
#define ADIS16_H

#ifndef ADIS16_FILE
    #define ADIS16_EXT extern
#else
    #define ADIS16_EXT
#endif

typedef struct {
    int32_t omega; // dir: clockwise
    int32_t theta;
    float temperature;

    int16_t omegaHW;
    // int32_t omegaInternal;
    // uint32_t thetaHW;
    // int16_t temperatureHW;

    uint32_t lastUpdateTick;
} ADIS16_DataTypeDef;

ADIS16_EXT volatile ADIS16_DataTypeDef ADIS16_Data;
ADIS16_EXT volatile uint8_t ADIS16_DataUpdated;

void ADIS16_Init(void);
void ADIS16_Update(void);
void ADIS16_Calibrate(uint16_t sample);

#endif // ADIS16_H
