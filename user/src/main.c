#include "main.h"

extern volatile uint32_t Count;
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
    ST7735_Print(0, 1, GREEN, BLACK, "Gyro Calibrate");

    SIMULATOR_CameraInit(6);

    CHASSIS_Init();
    ADIS16_Init();
    ADIS16_Calibrate(512);

    ST7735_Print(0, 1, GREEN, BLACK, "OMG");
    ST7735_Print(0, 2, GREEN, BLACK, "THE");
    ST7735_Print(0, 3, GREEN, BLACK, "I");
    ST7735_Print(0, 4, GREEN, BLACK, "V");
    ST7735_Print(0, 5, GREEN, BLACK, "P");
    ST7735_Print(0, 6, GREEN, BLACK, "RL");
    ST7735_Print(0, 7, GREEN, BLACK, "CNT");

    BSP_TIM_Start();

    while (1) {
        if (DBUS_Status == kLost) {
            CHASSIS_SetFree();
        }
        ST7735_Print(4, 1, GREEN, BLACK, "%d", ADIS16_Data.omega);
        ST7735_Print(4, 2, GREEN, BLACK, "%d", ADIS16_Data.theta);
        ST7735_Print(4, 3, GREEN, BLACK, "%.4f", JUDGE_Data.current);
        ST7735_Print(4, 4, GREEN, BLACK, "%.4f", JUDGE_Data.voltage);
        ST7735_Print(4, 5, GREEN, BLACK, "%.4f", JUDGE_Data.power);
        ST7735_Print(4, 6, GREEN, BLACK, "%d", JUDGE_Data.remainLife);
        ST7735_Print(4, 7, GREEN, BLACK, "%d", Count);
    }
}
