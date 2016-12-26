#ifndef PARAM_H
#define PARAM_H

#define CHASSIS_USE_3510_19

#if defined(CHASSIS_USE_3510_19)
/* CAN ID */
#define CHASSIS_MASTER_ID     0x200
#define CHASSIS_CAN_ID_OFFSET 0x201
#define FL_MOTOR_ID           0x201U
#define FR_MOTOR_ID           0x202U
#define BR_MOTOR_ID           0x203U
#define BL_MOTOR_ID           0x204U

#define MAX_TARGET_VELOCITY 8200

/* PID parameter */
#define CHASSIS_KP             5.20f
#define CHASSIS_KI             0.40f
#define CHASSIS_KD             0.08f
#define CHASSIS_MAX_POUT       10000
#define CHASSIS_MAX_INTEGRAL   25000
#define CHASSIS_MAX_PIDOUT     15000
#define CHASSIS_MIN_PIDOUT     0
#define CHASSIS_PID_MODE       kPositional

#elif defined(CHASSIS_USE_EC60)

#elif defined(CHASSIS_USE_RM35)

#endif // chassis motor parameter

#endif // PARAM_H
