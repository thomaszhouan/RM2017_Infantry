#define CHASSIS_FILE

#include "common.h"
#include "board_info.h"
#include "chassis.h"
#include <string.h>

#define CAN_ID_OFFSET 0x201
#define MAX_TARGET_VELOCITY 10000

#define _CLEAR(x) do { memset((void*)(x), 0, sizeof(x)); } while(0)

static PID_Controller MotorController[4];
static volatile uint16_t MotorAngle[4];
static int16_t MotorVelocityBuffer[4][4];
static uint8_t BufferId[4];
static volatile uint16_t MotorOutput[4];
static volatile char MeasureUpdated[4];

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
        MotorController[id].MIN_PIDout = 1000;
        MotorController[id].mode = kPositional;
        PID_Reset(MotorController+id);
    }
}

void CHASSIS_UpdateMeasure(uint16_t motorId) {
    uint16_t id = motorId - CAN_ID_OFFSET;
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
    uint16_t id = motorId - CAN_ID_OFFSET;
    if (!MeasureUpdated[id]) return;
    MotorOutput[id] = (int16_t)PID_Update(MotorController+id,
        TargetVelocity[id], MotorVelocity[id]>>2);
    MeasureUpdated[id] = 0;
}

void CHASSIS_SetTargetVelocity(uint16_t motorId, int16_t velocity) {
    uint16_t id = motorId - CAN_ID_OFFSET;
    velocity = CHASSIS_Trim(velocity, MAX_TARGET_VELOCITY);
    TargetVelocity[id] = velocity;
}

void CHASSIS_SendCmd(void) {
    uint8_t *data = CHASSIS_CAN_TX.Data;
    data[0] = (MotorOutput[0]&0xFF00)>>8;
    data[1] = MotorOutput[0]&0x00FF;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    data[5] = 0;
    data[6] = 0;
    data[7] = 0;
    HAL_CAN_Transmit_IT(&CHASSIS_CAN_HANDLE);
}

void CHASSIS_SetFree(void) {
    for (uint8_t i = 0; i < 4; ++i)
        PID_Reset(MotorController+i);
    CHASSIS_ClearAll();
}
