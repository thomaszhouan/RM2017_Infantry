#ifndef CAN_H
#define CAN_H

#include "common.h"

typedef struct {
    uint8_t id; // if id = x, CANx is used
    CAN_HandleTypeDef *CanHandle;
    CanRxMsgTypeDef *CanRx;
    CanTxMsgTypeDef *CanTx;
    uint32_t PreemptionPriority;
    uint32_t SubPriority;
} CAN_SimpleInitTypeDef;

void CAN_Init(CAN_SimpleInitTypeDef *canInit);

#endif // CAN_H
