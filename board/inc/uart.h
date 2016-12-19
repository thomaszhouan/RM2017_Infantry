#ifndef UART_H
#define UART_H

void UART_Init(UART_HandleTypeDef *handle, Uart_TypeDef uart, uint32_t baudrate);

uint16_t Strlen(const uint8_t *p);

#endif // UART_H
