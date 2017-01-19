#include "main.h"

int main(void) {
    BSP_GPIO_InitConfig();
    // BSP_CAN_InitConfig();
    BSP_TIM_InitConfig();
    // BSP_UART_InitConfig();
    // BSP_SPI_InitConfig();
    // BSP_DMA_InitConfig();
    BSP_NVIC_InitConfig();

    BSP_TIM_Start();

    while (1) {

    }
}
