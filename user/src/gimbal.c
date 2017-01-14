#define GIMBAL_FILE

#include "common.h"
#include "board_info.h"
#include "gimbal.h"
#include "can.h"
#include <string.h>

#define _ID(x) ((x)-GIMBAL_CAN_ID_OFFSET)

#define _CLEAR(x) do { memset((void*)(x), 0, sizeof(x)); } while(0)

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
    _CLEAR(GimbalMotorOutput);
}

void GIMBAL_Init(void) {
    GIMBAL_CANInit();

    GIMBAL_ClearAll();

    HAL_CAN_Receive_IT(&GIMBAL_CAN_HANDLE, 0);
}

void GIMBAL_UpdateMeasure(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    uint8_t *data = GIMBAL_CAN_RX.Data;
    GimbalPosition[id] = ((uint16_t)data[0]<<8)|((uint16_t)data[1]);
    GimbalRealCurrent[id] = ((uint16_t)data[2]<<8)|((uint16_t)data[3]);
    GimbalGivenCurrent[id] = ((uint16_t)data[4]<<8)|((uint16_t)data[5]);
}

void GIMBAL_SendCmd(void) {
    static uint8_t *data = GIMBAL_CAN_TX.Data;
    data[0] = (GimbalMotorOutput[0]&0xFF00)>>8;
    data[1] = GimbalMotorOutput[0]&0x00FF;
    data[2] = (GimbalMotorOutput[1]&0xFF00)>>8;
    data[3] = GimbalMotorOutput[1]&0x00FF;
    /* according to datasheet */
    data[5] = (GimbalMotorOutput[2]&0xFF00)>>8;
    data[4] = GimbalMotorOutput[2]&0x00FF;
    data[7] = (GimbalMotorOutput[3]&0xFF00)>>8;
    data[6] = GimbalMotorOutput[3]&0x00FF;
    HAL_CAN_Transmit_IT(&GIMBAL_CAN_HANDLE);
}
