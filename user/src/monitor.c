#include "common.h"
#include "board_info.h"
#include "monitor.h"
#include "uart.h"

void MONITOR_Init(void) {
    UART_SimpleInitTypeDef UART_InitStruct;
    UART_InitStruct.Instance               = MONITOR_UART;
    UART_InitStruct.UartHandle             = &MONITOR_UART_HANDLE;
    UART_InitStruct.DmaHandleTx            = &MONITOR_DMA_HANDLE;
    UART_InitStruct.DmaHandleRx            = NULL;
    UART_InitStruct.Baudrate               = 115200;
    UART_InitStruct.Parity                 = UART_PARITY_NONE;
    UART_InitStruct.PreemptionPriority     = 15;
    UART_InitStruct.SubPriority            = 0;
    UART_InitStruct.DMA_Tx_Mode            = DMA_NORMAL;
    UART_InitStruct.DMA_PreemptionPriority = 12;
    UART_InitStruct.DMA_SubPriority        = 0;
    UART_Init(&UART_InitStruct);
}

void MONITOR_Send(uint8_t *buffer, uint32_t length) {
    HAL_UART_Transmit_DMA(&MONITOR_UART_HANDLE, buffer, (uint16_t)length);
}
