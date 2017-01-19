#define GIMBAL_FILE

#include "common.h"
#include "board_info.h"
#include "gimbal.h"
#include "can.h"
#include "dbus.h"
#include "mpu6050.h"
#include <string.h>

#define _ID(x) ((x)-GIMBAL_CAN_ID_OFFSET)

#define _CLEAR(x) do { memset((void*)(x), 0, sizeof(x)); } while(0)

/* 1: normal 0: reverse */
static const char MOTOR_DIR[GIMBAL_MOTOR_CNT] = {0, 1, 1, 1};
static PID_Controller GimbalVelController[GIMBAL_MOTOR_CNT];
static uint16_t MeasureUpdated[GIMBAL_MOTOR_CNT];

static int32_t GIMBAL_Trim(int32_t val, int32_t lim) {
    if (val > lim) val = lim;
    if (val < -lim) val = -lim;
    return val;
}

static void GIMBAL_CANInit(void) {
    // CAN
    CAN_SimpleInitTypeDef CAN_InitStruct;
    CAN_InitStruct.id                 = GIMBAL_CAN_ID;
    CAN_InitStruct.CanHandle          = &GIMBAL_CAN_HANDLE;
    CAN_InitStruct.CanRx              = &GIMBAL_CAN_RX;
    CAN_InitStruct.CanTx              = &GIMBAL_CAN_TX;
    CAN_InitStruct.PreemptionPriority = 10;
    CAN_InitStruct.SubPriority        = 0;
    CAN_Init(&CAN_InitStruct);

    // CAN Tx
    GIMBAL_CAN_TX.StdId = GIMBAL_MASTER_ID;
    GIMBAL_CAN_TX.IDE   = CAN_ID_STD;
    GIMBAL_CAN_TX.RTR   = CAN_RTR_DATA;
    GIMBAL_CAN_TX.DLC   = 8;
}

static void GIMBAL_ClearAll(void) {
    GimbalPosition[0] = 7800;
    _CLEAR(GimbalLastPosition);
    _CLEAR(GimbalRoundCount);
    _CLEAR(GimbalOutput);
    _CLEAR(GimbalVelocity);
    _CLEAR(GimbalRealCurrent);
    _CLEAR(GimbalGivenCurrent);
    _CLEAR(TargetVelocity);
}

void GIMBAL_Init(void) {
    GIMBAL_CANInit();

    GIMBAL_ClearAll();
    for (uint8_t i = 0; i < GIMBAL_MOTOR_CNT; ++i)
        PID_Reset(GimbalVelController+i);
    GimbalVelController[YAW].Kp = 0.145f;
    GimbalVelController[YAW].Ki = 0.10f;//0.04f;
    GimbalVelController[YAW].Kd = 0.00f;
    GimbalVelController[YAW].MAX_Pout = 2000;
    GimbalVelController[YAW].MAX_Integral = 1000;
    GimbalVelController[YAW].MAX_PIDout = 5000;
    GimbalVelController[YAW].MIN_PIDout = 0;
    // GimbalVelController[YAW].IDecayFactor = 0.9;
    GimbalVelController[YAW].mode = kPositional;

    HAL_CAN_Receive_IT(&GIMBAL_CAN_HANDLE, 0);
}

void GIMBAL_UpdateMeasure(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    uint8_t *data = GIMBAL_CAN_RX.Data;
    GimbalLastPosition[id] = GimbalPosition[id];
    GimbalPosition[id] = ((uint16_t)data[0]<<8)|((uint16_t)data[1]);
    GimbalRealCurrent[id] = ((uint16_t)data[2]<<8)|((uint16_t)data[3]);
    GimbalGivenCurrent[id] = ((uint16_t)data[4]<<8)|((uint16_t)data[5]);

    GimbalVelocity[id] = GimbalPosition[id] - GimbalLastPosition[id];
    if (GimbalVelocity[id] > 5000) {
        --GimbalRoundCount[id];
        GimbalPosition[id] -= 8192;
        GimbalVelocity[id] -= 8192;
    }
    else if (GimbalVelocity[id] < -5000) {
        ++GimbalRoundCount[id];
        GimbalPosition[id] += 8192;
        GimbalVelocity[id] += 8192;
    }

    MeasureUpdated[id] = 1;
}

void GIMBAL_SendCmd(void) {
    static uint8_t *data = GIMBAL_CAN_TX.Data;
    data[0] = (GimbalOutput[0]&0xFF00)>>8;
    data[1] = GimbalOutput[0]&0x00FF;
    data[2] = (GimbalOutput[1]&0xFF00)>>8;
    data[3] = GimbalOutput[1]&0x00FF;
    /* according to datasheet */
    data[5] = (GimbalOutput[2]&0xFF00)>>8;
    data[4] = GimbalOutput[2]&0x00FF;
    data[7] = (GimbalOutput[3]&0xFF00)>>8;
    data[6] = GimbalOutput[3]&0x00FF;
    HAL_CAN_Transmit_IT(&GIMBAL_CAN_HANDLE);
}

void GIMBAL_MotorControl(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    if (!MeasureUpdated[id]) return;
    // GimbalOutput[id] = DBUS_Data.ch3;
    GimbalOutput[id] = -(int16_t)PID_Update(GimbalVelController+id,
        TargetVelocity[id], MPU6050_GyroData[2]);

    if (GimbalPosition[0] > 9800 && GimbalOutput[0] < 0)
        GimbalOutput[0] = 0;
    else if (GimbalPosition[0] < 5400 && GimbalOutput[0] > 0)
        GimbalOutput[0] = 0;

    MeasureUpdated[id] = 0;
}

void GIMBAL_Control(void) {
    GIMBAL_MotorControl(GIMBAL_YAW_ID);
    GIMBAL_MotorControl(GIMBAL_PITCH_ID);
}

/*
    +ve direction
    YAW: CW
    PITCH:
*/
void GIMBAL_SetMotion(void) {
    TargetVelocity[0] = -DBUS_Data.ch3 * 4;
}

void GIMBAL_SetFree(void) {
    for (uint8_t i = 0; i < GIMBAL_MOTOR_CNT; ++i)
        PID_Reset(GimbalVelController+i);
    GIMBAL_ClearAll();
}
