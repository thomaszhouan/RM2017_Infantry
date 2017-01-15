#ifndef MONITOR_H
#define MONITOR_H

#include "board_info.h"

#define MONITOR_UART                            EVALUATOR2(UART, MONITOR_UART_NUM)
#define MONITOR_UART_HANDLE                     EVALUATOR3(Uart, MONITOR_UART_NUM, _Handle)
#define MONITOR_DMA_HANDLE                      EVALUATOR3(Uart, MONITOR_UART_NUM, _TxDmaHandle)

void MONITOR_Init(void);
void MONITOR_Send(uint8_t *buffer, uint32_t length);

#endif // MONITOR_H
