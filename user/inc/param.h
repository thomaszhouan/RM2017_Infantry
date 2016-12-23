#ifndef PARAM_H
#define PARAM_H

#define CHASSIS_USE_3510_19

#if defined(CHASSIS_USE_3510_19)
#define CHASSIS_CAN_ID_OFFSET 0x201
#define FL_MOTOR_ID 0x201U
#define FR_MOTOR_ID 0x202U
#define BR_MOTOR_ID 0x203U
#define BL_MOTOR_ID 0x204U

#elif defined(CHASSIS_USE_EC60)

#elif defined(CHASSIS_USE_RM35)

#endif // chassis motor parameter

#endif // PARAM_H
