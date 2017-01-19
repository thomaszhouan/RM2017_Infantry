#include "main.h"

int main(void) {
    BSP_GPIO_InitConfig();
    BSP_CAN_InitConfig();
    BSP_TIM_InitConfig();
    BSP_UART_InitConfig();
    BSP_SPI_InitConfig();
    BSP_DMA_InitConfig();
    BSP_NVIC_InitConfig();
    BSP_DWT_InitConfig();

    ST7735_Init();
    ST7735_SetOrientation(kRevert);
    ST7735_FillColor(BLACK);
    ST7735_Print(0, 0, GREEN, BLACK, "RM2017 Infantry");
    ST7735_Print(0, 1, GREEN, BLACK, "OMG");
    ST7735_Print(0, 2, GREEN, BLACK, "THE");

    CHASSIS_Init();
    ADIS16_Init();
    ADIS16_Calibrate(512);

    BSP_TIM_Start();

    while (1) {
        if (DBUS_Status == kLost) {
            CHASSIS_SetFree();
        }
        ST7735_Print(4, 1, GREEN, BLACK, "%d", ADIS16_Data.omega);
        ST7735_Print(4, 2, GREEN, BLACK, "%d", ADIS16_Data.theta);
    }
}
