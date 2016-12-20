/*
    handle.c
    This file contains the definition of global handles
*/

#include "common.h"

// USART1
UART_HandleTypeDef Uart1_Handle;
DMA_HandleTypeDef Uart1_TxDmaHandle, Uart1_RxDmaHandle;

// USART3
UART_HandleTypeDef Uart3_Handle;
DMA_HandleTypeDef Uart3_TxDmaHandle, Uart3_RxDmaHandle;

// TIM2
TIM_HandleTypeDef Tim2_Handle;
