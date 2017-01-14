#ifndef GIMBAL_H
#define GIMBAL_H

#include "pid.h"
#include "param.h"

#ifndef GIMBAL_FILE
    #define GIMBAL_EXT extern
#else
    #define GIMBAL_EXT
#endif // GIMBAL_FILE

GIMBAL_EXT volatile int16_t GimbalMotorOutput[GIMBAL_MOTOR_CNT]; // [-5000, 5000]
GIMBAL_EXT volatile uint16_t GimbalPosition[GIMBAL_MOTOR_CNT]; // [0, 8191]
GIMBAL_EXT volatile int16_t GimbalRealCurrent[GIMBAL_MOTOR_CNT]; // [-13000, 13000]
GIMBAL_EXT volatile int16_t GimbalGivenCurrent[GIMBAL_MOTOR_CNT]; // [-5000, 5000]

void GIMBAL_Init(void);
void GIMBAL_UpdateMeasure(uint16_t motorId);
void GIMBAL_SendCmd(void);

#endif // GIMBAL_H
