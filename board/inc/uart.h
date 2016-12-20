#ifndef UART_H
#define UART_H

typedef struct {
    Uart_TypeDef Instance;
    UART_HandleTypeDef *UartHandle;
    DMA_HandleTypeDef *DmaHandleTx;
    DMA_HandleTypeDef *DmaHandleRx;
    uint32_t Baudrate;
    uint32_t Parity;
    uint32_t PreemptionPriority;
    uint32_t SubPriority;
    uint32_t DMA_Tx_Mode;
    uint32_t DMA_Rx_Mode;
    uint32_t DMA_PreemptionPriority;
    uint32_t DMA_SubPriority;
} UART_SimpleInitTypeDef;

void UART_Init(UART_SimpleInitTypeDef *UART_Init);

uint16_t Strlen(const uint8_t *p);

#endif // UART_H
