#include "common.h"
#include "board_info.h"
#include "uart.h"

static GPIO_TypeDef* GPIO_PORT[UARTn]      = {UART1_PORT, UART3_PORT};
static const uint16_t GPIO_PIN[UARTn]      = {UART1_TX_PIN | UART1_RX_PIN,
                                              UART3_TX_PIN | UART3_RX_PIN};
static const uint32_t GPIO_AF[UARTn]       = {GPIO_AF7_USART1, GPIO_AF7_USART3};
static USART_TypeDef* UART_INSTANCE[UARTn] = {USART1, USART3};

void UART_Init(UART_HandleTypeDef *handle, Uart_TypeDef uart, uint32_t baudrate) {
    UARTx_CLK_ENABLE(uart);
    UARTx_GPIO_CLK_ENABLE(uart);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin   = GPIO_PIN[uart];
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF[uart];
    HAL_GPIO_Init(GPIO_PORT[uart], &GPIO_InitStruct);

    // default: 8-N-1, no flow control
    handle->Instance = UART_INSTANCE[uart];
    handle->Init.BaudRate     = baudrate;
    handle->Init.WordLength   = UART_WORDLENGTH_8B;
    handle->Init.StopBits     = UART_STOPBITS_1;
    handle->Init.Parity       = UART_PARITY_NONE;
    handle->Init.Mode         = UART_MODE_TX_RX;
    handle->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    handle->Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(handle);
}

uint16_t Strlen(const uint8_t *p) {
    uint16_t ret = 0;
    while (*p++)
        ++ret;
    return ret;
}
