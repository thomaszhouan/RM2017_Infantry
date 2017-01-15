#define GIMBAL_FILE

#include "common.h"
#include "board_info.h"
#include "gimbal.h"
#include "can.h"
#include "dbus.h"
#include <string.h>

#define _ID(x) ((x)-GIMBAL_CAN_ID_OFFSET)

#define _CLEAR(x) do { memset((void*)(x), 0, sizeof(x)); } while(0)

static PID_Controller GimbalController[4];

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
    _CLEAR(GimbalPosition);
    _CLEAR(GimbalLastPosition);
    _CLEAR(GimbalOutput);
    _CLEAR(GimbalVelocity);
    _CLEAR(GimbalRealCurrent);
    _CLEAR(GimbalGivenCurrent);
    _CLEAR(GimbalPositionBuffer);
    _CLEAR(GimbalPositionBufferId);
}

void GIMBAL_Init(void) {
    GIMBAL_CANInit();

    GIMBAL_ClearAll();

    HAL_CAN_Receive_IT(&GIMBAL_CAN_HANDLE, 0);
}

void GIMBAL_UpdateMeasure(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    uint8_t *data = GIMBAL_CAN_RX.Data;
    GimbalLastPosition[id] = GimbalPosition[id];
    GimbalPosition[id] = ((uint16_t)data[0]<<8)|((uint16_t)data[1]);
    GimbalRealCurrent[id] = ((uint16_t)data[2]<<8)|((uint16_t)data[3]);
    GimbalGivenCurrent[id] = ((uint16_t)data[4]<<8)|((uint16_t)data[5]);

    for (uint8_t i = 0; i < POSITION_BUFFER_SIZE-1; ++i)
        GimbalPositionBuffer[id][i] = GimbalPositionBuffer[id][i+1];
    GimbalPositionBuffer[id][POSITION_BUFFER_SIZE-1] = GimbalPosition[id];
    int32_t sum = 0, tmp_v;
    for (uint8_t i = 0; i < POSITION_BUFFER_SIZE/2; ++i) {
        tmp_v = GimbalPositionBuffer[id][i+POSITION_BUFFER_SIZE/2]-GimbalPositionBuffer[id][i];
        if (tmp_v > 4000) tmp_v -= 8192;
        else if (tmp_v < -4000) tmp_v += 8192;
        sum += tmp_v;
    }
    GimbalVelocity[id] = (sum * 4) / (POSITION_BUFFER_SIZE * POSITION_BUFFER_SIZE);

    // int16_t tmp_v1 = GimbalPosition[id] - GimbalLastPosition[id];
    // int16_t tmp_v2 = tmp_v1 + 8192;
    // GimbalVelocity[id] = (ABS(tmp_v1) < ABS(tmp_v2)) ? tmp_v1 : tmp_v2;
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

void GIMBAL_Control(void) {
    GimbalOutput[0] = DBUS_Data.ch3*7;
}

void GIMBAL_SetFree(void) {
    GIMBAL_ClearAll();
}
