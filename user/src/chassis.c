#define CHASSIS_FILE

#include "common.h"
#include "board_info.h"
#include "chassis.h"
#include "dbus.h"
#include "can.h"
#include <string.h>

#define MAX_TARGET_VELOCITY 8200

#define _ID(x) ((x)-CHASSIS_CAN_ID_OFFSET)

/* 1: normal 0: reverse */
static const char MOTOR_DIR[4] = {1, 0, 0, 1};

#define _CLEAR(x) do { memset((void*)(x), 0, sizeof(x)); } while(0)

static int32_t CHASSIS_Trim(int32_t val, int32_t lim) {
    if (val > lim) val = lim;
    if (val < -lim) val = -lim;
    return val;
}

static void CHASSIS_ClearAll(void) {
    _CLEAR(MotorAngle);
    _CLEAR(MotorLastAngle);
    _CLEAR(MotorVelocity);
    _CLEAR(MotorVelocityBuffer);
    _CLEAR(BufferId);
    _CLEAR(TargetVelocity);
    _CLEAR(MotorOutput);
    _CLEAR(MeasureUpdated);
}

static void CHASSIS_CanInit(void) {
    // CAN
    CAN_SimpleInitTypeDef CAN_InitStruct;
    CAN_InitStruct.id                 = CHASSIS_CAN_ID;
    CAN_InitStruct.CanHandle          = &CHASSIS_CAN_HANDLE;
    CAN_InitStruct.CanRx              = &CHASSIS_CAN_RX;
    CAN_InitStruct.CanTx              = &CHASSIS_CAN_TX;
    CAN_InitStruct.PreemptionPriority = 9;
    CAN_InitStruct.SubPriority        = 0;
    CAN_Init(&CAN_InitStruct);

    // CAN Tx
    CHASSIS_CAN_TX.StdId = CHASSIS_MASTER_ID;
    CHASSIS_CAN_TX.IDE   = CAN_ID_STD;
    CHASSIS_CAN_TX.RTR   = CAN_RTR_DATA;
    CHASSIS_CAN_TX.DLC   = 8;
}

void CHASSIS_Init(void) {
    CHASSIS_CanInit();

    for (uint8_t id = 0; id < 4; ++id) {
        MotorController[id].Kp = CHASSIS_KP;
        MotorController[id].Ki = CHASSIS_KI;
        MotorController[id].Kd = CHASSIS_KD;
        MotorController[id].MAX_Pout = CHASSIS_MAX_POUT;
        MotorController[id].MAX_Iout = CHASSIS_MAX_IOUT;
        MotorController[id].MAX_PIDout = CHASSIS_MAX_PIDOUT;
        MotorController[id].MIN_PIDout = CHASSIS_MIN_PIDOUT;
        MotorController[id].mode = CHASSIS_PID_MODE;
        PID_Reset(MotorController+id);
    }
    CHASSIS_ClearAll();

    HAL_CAN_Receive_IT(&CHASSIS_CAN_HANDLE, 0);
}

void CHASSIS_UpdateMeasure(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    uint8_t *data = CHASSIS_CAN_RX.Data;
    MotorLastAngle[id] = MotorAngle[id];
    MotorAngle[id] = ((uint16_t)data[0]<<8) | ((uint16_t)data[1]);
    int16_t newVelocity = ((uint16_t)data[2]<<8) | ((uint16_t)data[3]);

    /* overflow protection */
    if (TargetVelocity[id] > 10 && newVelocity < -3000) newVelocity += 16384;
    else if (TargetVelocity[id] < -10 && newVelocity > 3000) newVelocity -= 16384;

    MotorVelocity[id] -= MotorVelocityBuffer[id][BufferId[id]];
    MotorVelocityBuffer[id][BufferId[id]] = newVelocity;
    MotorVelocity[id] += newVelocity;
    BufferId[id] = (BufferId[id]+1)&0x1U;

    MeasureUpdated[id] = 1;
}

void CHASSIS_MotorControl(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    if (!MeasureUpdated[id]) return;
    MotorOutput[id] = (int16_t)PID_Update(MotorController+id,
        TargetVelocity[id], MotorVelocity[id]/2);

    MeasureUpdated[id] = 0;
}

void CHASSIS_Control(void) {
    CHASSIS_MotorControl(FL_MOTOR_ID);
    CHASSIS_MotorControl(FR_MOTOR_ID);
    CHASSIS_MotorControl(BR_MOTOR_ID);
    CHASSIS_MotorControl(BL_MOTOR_ID);
}

/*
    y
    ^
    |
    0-- >x
*/
void CHASSIS_SetMotion(void) {
    static int32_t velocityX = 0, velocityY = 0, omega = 0;
    static int32_t tmpVelocity[4];

    /* low pass filter */
    velocityX = (velocityX*7+6*DBUS_Data.ch1)/8;
    velocityY = (velocityY*7+12*DBUS_Data.ch2)/8;
    omega = (omega*7+8*DBUS_Data.ch3)/8;

    tmpVelocity[0] = (int32_t)velocityY + velocityX + omega;
    tmpVelocity[1] = (int32_t)velocityY - velocityX - omega;
    tmpVelocity[2] = (int32_t)velocityY + velocityX - omega;
    tmpVelocity[3] = (int32_t)velocityY - velocityX + omega;

    for (uint8_t i = 0; i < 4; ++i)
        CHASSIS_SetTargetVelocity(i+CHASSIS_CAN_ID_OFFSET, tmpVelocity[i]);
}

void CHASSIS_SetTargetVelocity(uint16_t motorId, int32_t velocity) {
    uint16_t id = _ID(motorId);
    velocity = CHASSIS_Trim(velocity, MAX_TARGET_VELOCITY);
    TargetVelocity[id] = (int16_t)velocity;
    if (!MOTOR_DIR[id]) TargetVelocity[id] = -TargetVelocity[id];
}

void CHASSIS_SendCmd(void) {
    static uint8_t *data = CHASSIS_CAN_TX.Data;
    data[0] = (MotorOutput[0]&0xFF00)>>8;
    data[1] = MotorOutput[0]&0x00FF;
    data[2] = (MotorOutput[1]&0xFF00)>>8;
    data[3] = MotorOutput[1]&0x00FF;
    data[4] = (MotorOutput[2]&0xFF00)>>8;
    data[5] = MotorOutput[2]&0x00FF;
    data[6] = (MotorOutput[3]&0xFF00)>>8;
    data[7] = MotorOutput[3]&0x00FF;
    HAL_CAN_Transmit_IT(&CHASSIS_CAN_HANDLE);
}

void CHASSIS_SetFree(void) {
    for (uint8_t i = 0; i < 4; ++i)
        PID_Reset(MotorController+i);
    CHASSIS_ClearAll();
}
