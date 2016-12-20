#include "common.h"
#include "board_info.h"
#include "uart.h"

static GPIO_TypeDef* const GPIO_PORT[UARTn] = {UART1_PORT, UART3_PORT};
static const uint16_t GPIO_PIN[UARTn] = {UART1_TX_PIN | UART1_RX_PIN,
                                         UART3_TX_PIN | UART3_RX_PIN};
static const uint32_t GPIO_AF[UARTn] = {GPIO_AF7_USART1, GPIO_AF7_USART3};
static USART_TypeDef* const UART_INSTANCE[UARTn] = {USART1, USART3};
static const IRQn_Type UART_IRQn[UARTn] = {USART1_IRQn, USART3_IRQn};
static DMA_Stream_TypeDef* const DMA_INSTANCE[UARTn<<1] = {DMA2_Stream7, DMA2_Stream5,
                                                           DMA1_Stream3, DMA1_Stream1};
static const uint32_t DMA_CHANNEL[UARTn] = {DMA_CHANNEL_4, DMA_CHANNEL_4};
static const IRQn_Type DMA_IRQn[UARTn << 1] = {DMA2_Stream7_IRQn, DMA2_Stream5_IRQn,
                                               DMA1_Stream3_IRQn, DMA1_Stream1_IRQn};

void UART_Init(UART_SimpleInitTypeDef *UART_Init) {
    Uart_TypeDef uart = UART_Init->Instance;
    UARTx_CLK_ENABLE(uart);
    UARTx_GPIO_CLK_ENABLE(uart);

    // init GPIO
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin       = GPIO_PIN[uart];
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = GPIO_AF[uart];
    HAL_GPIO_Init(GPIO_PORT[uart], &GPIO_InitStruct);

    // default: no flow control
    UART_HandleTypeDef *handle = UART_Init->UartHandle;
    handle->Instance          = UART_INSTANCE[uart];
    handle->Init.BaudRate     = UART_Init->Baudrate;
    handle->Init.WordLength   = (UART_Init->Parity==UART_PARITY_NONE)?
                                UART_WORDLENGTH_8B:UART_WORDLENGTH_9B;
    handle->Init.StopBits     = UART_STOPBITS_1;
    handle->Init.Parity       = UART_Init->Parity;
    handle->Init.Mode         = UART_MODE_TX_RX;
    handle->Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    handle->Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(handle);

    HAL_NVIC_SetPriority(UART_IRQn[uart], UART_Init->PreemptionPriority,
        UART_Init->SubPriority);
    HAL_NVIC_EnableIRQ(UART_IRQn[uart]);

    UARTx_DMA_CLK_ENABLE(uart);
    DMA_HandleTypeDef *hdma;
    // DMA Tx
    hdma = UART_Init->DmaHandleTx;
    if (hdma != NULL) {
        hdma->Init.Channel             = DMA_CHANNEL[uart];
        hdma->Init.Direction           = DMA_MEMORY_TO_PERIPH;
        hdma->Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma->Init.MemInc              = DMA_MINC_ENABLE;
        hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma->Init.Mode                = UART_Init->DMA_Tx_Mode;
        hdma->Init.Priority            = DMA_PRIORITY_HIGH;
        hdma->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma->Init.PeriphBurst         = DMA_PBURST_SINGLE;
        hdma->Init.MemBurst            = DMA_MBURST_SINGLE;
        hdma->Parent                   = handle;
        hdma->Instance                 = DMA_INSTANCE[uart<<1];
        HAL_DMA_Init(hdma);
        handle->hdmatx = hdma;
        HAL_NVIC_SetPriority(DMA_IRQn[uart<<1], UART_Init->DMA_PreemptionPriority,
            UART_Init->DMA_SubPriority);
        HAL_NVIC_EnableIRQ(DMA_IRQn[uart<<1]);
    }

    // Rx
    hdma = UART_Init->DmaHandleRx;
    if (hdma != NULL) {
        hdma->Init.Channel             = DMA_CHANNEL[uart];
        hdma->Init.Direction           = DMA_PERIPH_TO_MEMORY;
        hdma->Init.PeriphInc           = DMA_PINC_DISABLE;
        hdma->Init.MemInc              = DMA_MINC_ENABLE;
        hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma->Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        hdma->Init.Mode                = UART_Init->DMA_Rx_Mode;
        hdma->Init.Priority            = DMA_PRIORITY_HIGH;
        hdma->Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        hdma->Init.PeriphBurst         = DMA_PBURST_SINGLE;
        hdma->Init.MemBurst            = DMA_MBURST_SINGLE;
        hdma->Parent                   = handle;
        hdma->Instance                 = DMA_INSTANCE[(uart<<1)+1];
        HAL_DMA_Init(hdma);
        handle->hdmarx = hdma;
        HAL_NVIC_SetPriority(DMA_IRQn[(uart<<1)+1], UART_Init->DMA_PreemptionPriority,
            UART_Init->DMA_SubPriority);
        HAL_NVIC_EnableIRQ(DMA_IRQn[(uart<<1)+1]);
    }
}

uint16_t Strlen(const uint8_t *p) {
    uint16_t ret = 0;
    while (*p++)
        ++ret;
    return ret;
}
