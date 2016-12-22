#ifndef CHASSIS_H
#define CHASSIS_H

#include "pid.h"

#define FL_MOTOR_ID 0x201U
#define FR_MOTOR_ID 0x202U
#define BL_MOTOR_ID 0x203U
#define BR_MOTOR_ID 0x204U

#ifndef CHASSIS_FILE
    #define CHASSIS_EXT extern
#else
    #define CHASSIS_EXT
#endif

CHASSIS_EXT volatile int16_t MotorVelocity[4], TargetVelocity[4];

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
    Update velocity target.
*/
void CHASSIS_SetTargetVelocity(uint16_t motorId, int16_t velocity);

/*
    Send CAN cmd.
*/
void CHASSIS_SendCmd(void);

/*
    Send zero to all chassis motors.
*/
void CHASSIS_SetFree(void);

#endif // CHASSIS_H
