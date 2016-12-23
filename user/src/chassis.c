#define CHASSIS_FILE

#include "common.h"
#include "board_info.h"
#include "chassis.h"
#include "dbus.h"
#include <string.h>

#define MAX_TARGET_VELOCITY 10000

#define _ID(x) ((x)-CHASSIS_CAN_ID_OFFSET)

// 1: normal 0: reverse
static const char MOTOR_DIR[4] = {1, 0, 0, 1};

#define _CLEAR(x) do { memset((void*)(x), 0, sizeof(x)); } while(0)

static int16_t CHASSIS_Trim(int16_t val, int16_t lim) {
    if (val > lim) val = lim;
    if (val < -lim) val = -lim;
    return val;
}

static void CHASSIS_ClearAll(void) {
    _CLEAR(MotorAngle);
    _CLEAR(MotorVelocity);
    _CLEAR(MotorVelocityBuffer);
    _CLEAR(BufferId);
    _CLEAR(TargetVelocity);
    _CLEAR(MotorOutput);
    _CLEAR(MeasureUpdated);
}

void CHASSIS_Init(void) {
    for (uint8_t id = 0; id < 4; ++id) {
        MotorController[id].Kp = 8.00f;
        MotorController[id].Ki = 0.55f;
        MotorController[id].Kd = 0.05f;
        MotorController[id].MAX_Iout = 30000;
        MotorController[id].MAX_PIDout = 30000;
        MotorController[id].MIN_PIDout = 2000;
        MotorController[id].mode = kPositional;
        PID_Reset(MotorController+id);
    }
}

void CHASSIS_UpdateMeasure(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    uint8_t *data = CHASSIS_CAN_RX.Data;
    MotorAngle[id] = ((uint16_t)data[0]<<8) | ((uint16_t)data[1]);
    int16_t newVelocity = ((uint16_t)data[2]<<8) | ((uint16_t)data[3]);
    MotorVelocity[id] -= MotorVelocityBuffer[id][BufferId[id]];
    MotorVelocityBuffer[id][BufferId[id]] = newVelocity;
    MotorVelocity[id] += newVelocity;
    ++BufferId[id];
    BufferId[id] &= 0x03;

    MeasureUpdated[id] = 1;
}

void CHASSIS_MotorControl(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    if (!MeasureUpdated[id]) return;
    MotorOutput[id] = (int16_t)PID_Update(MotorController+id,
        TargetVelocity[id], MotorVelocity[id]>>2);

    MeasureUpdated[id] = 0;
}

/*
    y
    ^
    |
    0-- >x
*/
void CHASSIS_SetMotion(void) {
    static int32_t velocityX, velocityY, omega;
    static int32_t tmpVelocity[4];

    velocityX = (velocityX*7+8*DBUS_Data.ch1)/8;
    velocityY = (velocityY*7+6*DBUS_Data.ch2)/8;
    omega = (omega*7+8*DBUS_Data.ch3)/8;

    tmpVelocity[0] = velocityY + velocityX + omega;
    tmpVelocity[1] = velocityY - velocityX - omega;
    tmpVelocity[2] = velocityY + velocityX - omega;
    tmpVelocity[3] = velocityY - velocityX + omega;

    for (uint8_t i = 0; i < 4; ++i)
        CHASSIS_SetTargetVelocity(i+CHASSIS_CAN_ID_OFFSET, tmpVelocity[i]);
}

void CHASSIS_SetTargetVelocity(uint16_t motorId, int16_t velocity) {
    uint16_t id = _ID(motorId);
    velocity = CHASSIS_Trim(velocity, MAX_TARGET_VELOCITY);
    TargetVelocity[id] = velocity;
    if (!MOTOR_DIR[id]) TargetVelocity[id] = -TargetVelocity[id];
}

void CHASSIS_SendCmd(void) {
    uint8_t *data = CHASSIS_CAN_TX.Data;
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
