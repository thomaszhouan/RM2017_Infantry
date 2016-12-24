#ifndef CHASSIS_H
#define CHASSIS_H

#include "pid.h"
#include "param.h"

#ifndef CHASSIS_FILE
    #define CHASSIS_EXT extern
#else
    #define CHASSIS_EXT
#endif

CHASSIS_EXT volatile int32_t MotorVelocity[4], TargetVelocity[4];
CHASSIS_EXT PID_Controller MotorController[4];
CHASSIS_EXT volatile int16_t MotorAngle[4], MotorLastAngle[4];
CHASSIS_EXT int16_t MotorVelocityBuffer[4][4];
CHASSIS_EXT uint8_t BufferId[4];
CHASSIS_EXT volatile int16_t MotorOutput[4];
CHASSIS_EXT volatile char MeasureUpdated[4];

/*
    Initialize controllers and internal states.
*/
void CHASSIS_Init(void);

/*
    Update motor angle & velocity.
*/
void CHASSIS_UpdateMeasure(uint16_t motorId);

/*
    Update the controller if angle/velocity updated.
*/
void CHASSIS_MotorControl(uint16_t motorId);

/*
    Set target velocity for 4 motors.
*/
void CHASSIS_SetMotion(void);

/*
    Update velocity target.
*/
void CHASSIS_SetTargetVelocity(uint16_t motorId, int32_t velocity);

/*
    Send CAN cmd.
*/
void CHASSIS_SendCmd(void);

/*
    Send zero to all chassis motors.
*/
void CHASSIS_SetFree(void);

#endif // CHASSIS_H
