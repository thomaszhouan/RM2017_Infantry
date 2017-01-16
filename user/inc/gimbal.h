#ifndef GIMBAL_H
#define GIMBAL_H

#include "pid.h"
#include "param.h"

#ifndef GIMBAL_FILE
    #define GIMBAL_EXT extern
#else
    #define GIMBAL_EXT
#endif // GIMBAL_FILE

#define POSITION_BUFFER_SIZE                    4
#define MAX_YAW_VELOCITY                        40

GIMBAL_EXT volatile int16_t GimbalOutput[GIMBAL_MOTOR_CNT]; // [-5000, 5000]
GIMBAL_EXT volatile int16_t GimbalPosition[GIMBAL_MOTOR_CNT], GimbalLastPosition[GIMBAL_MOTOR_CNT]; // [0, 8191]
GIMBAL_EXT volatile int32_t GimbalPositionBuffer[GIMBAL_MOTOR_CNT][POSITION_BUFFER_SIZE];
GIMBAL_EXT volatile int8_t GimbalPositionBufferId[GIMBAL_MOTOR_CNT];
GIMBAL_EXT volatile int16_t GimbalRealCurrent[GIMBAL_MOTOR_CNT]; // [-13000, 13000]
GIMBAL_EXT volatile int16_t GimbalGivenCurrent[GIMBAL_MOTOR_CNT]; // [-5000, 5000]
GIMBAL_EXT volatile int32_t GimbalVelocity[GIMBAL_MOTOR_CNT];
GIMBAL_EXT volatile int16_t TargetVelocity[GIMBAL_MOTOR_CNT];

void GIMBAL_Init(void);
void GIMBAL_UpdateMeasure(uint16_t motorId);
void GIMBAL_SendCmd(void);
void GIMBAL_MotorControl(uint16_t motorId);
void GIMBAL_Control(void);
void GIMBAL_SetMotion(void);
void GIMBAL_SetFree(void);

#endif // GIMBAL_H
