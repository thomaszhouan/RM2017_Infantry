#include "main.h"

int main(void) {
    BSP_GPIO_InitConfig();
    // BSP_CAN_InitConfig();
    BSP_TIM_InitConfig();
    // BSP_UART_InitConfig();
    BSP_SPI_InitConfig();
    // BSP_DMA_InitConfig();
    BSP_NVIC_InitConfig();

    ST7735_Init();
    ST7735_SetOrientation(kRevert);
    ST7735_FillColor(BLACK);
    ST7735_Print(0, 0, GREEN, BLACK, "RM2017 Infantry");

    BSP_TIM_Start();

    while (1) {

    }
}
