#include "common.h"
#include "board_info.h"
#include "can.h"

/*
    Rx0 is used.
*/
static GPIO_TypeDef* const GPIO_PORT[xCANn] = {CAN1_GPIO_PORT, CAN2_GPIO_PORT};
static const uint16_t GPIO_PIN[xCANn] = {CAN1_GPIO_PIN, CAN2_GPIO_PIN};
static CAN_TypeDef* const CAN_INSTANCE[xCANn] = {CAN1, CAN2};
static const IRQn_Type xCAN_IRQn[xCANn<<1] = {CAN1_TX_IRQn, CAN1_RX0_IRQn,
                                              CAN2_TX_IRQn, CAN2_RX0_IRQn};
static const xCAN_TypeDef CAN_ID[xCANn] = {xCAN1, xCAN2};

void CAN_Init(CAN_SimpleInitTypeDef *canInit) {
    uint8_t id = CAN_ID[canInit->id-1];

    CANx_GPIO_CLK_ENABLE(id);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN[id];
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIO_PORT[id], &GPIO_InitStruct);

    /* 
        PCLK = 42MHz
        CAN clk = 14MHz
        Bit time = 14TQ
        Bit rate = 1Mbps
    */
    xCAN_CLK_ENABLE();
    CAN_HandleTypeDef *handle = canInit->CanHandle;
    handle->Instance = CAN_INSTANCE[id];
    handle->Init.Prescaler = 3;
    handle->Init.Mode = CAN_MODE_NORMAL;
    handle->Init.SJW = CAN_SJW_1TQ;
    handle->Init.BS1 = CAN_BS1_9TQ;
    handle->Init.BS2 = CAN_BS2_4TQ;
    handle->Init.TTCM = DISABLE;
    handle->Init.ABOM = DISABLE;
    handle->Init.AWUM = DISABLE;
    handle->Init.NART = DISABLE;
    handle->Init.RFLM = DISABLE;
    handle->Init.TXFP = ENABLE;
    handle->pTxMsg = canInit->CanTx;
    handle->pRxMsg = canInit->CanRx;
    HAL_CAN_Init(handle);

    /*
        Nothing is filtered.
    */
    CAN_FilterConfTypeDef CAN_FilterConfStruct;
    CAN_FilterConfStruct.FilterIdHigh = 0x0000U;
    CAN_FilterConfStruct.FilterIdLow = 0x0000U;
    CAN_FilterConfStruct.FilterMaskIdHigh = 0x0000U;
    CAN_FilterConfStruct.FilterMaskIdLow = 0x0000U;
    CAN_FilterConfStruct.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    CAN_FilterConfStruct.FilterNumber = 0;
    CAN_FilterConfStruct.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN_FilterConfStruct.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN_FilterConfStruct.FilterActivation = ENABLE;
    CAN_FilterConfStruct.BankNumber = 0;
    HAL_CAN_ConfigFilter(handle, &CAN_FilterConfStruct);

    HAL_NVIC_SetPriority(xCAN_IRQn[id<<1], canInit->PreemptionPriority,
        canInit->SubPriority);
    HAL_NVIC_SetPriority(xCAN_IRQn[(id<<1)+1], canInit->PreemptionPriority,
        canInit->SubPriority);
    HAL_NVIC_EnableIRQ(xCAN_IRQn[id<<1]);
    HAL_NVIC_EnableIRQ(xCAN_IRQn[(id<<1)+1]);
}
