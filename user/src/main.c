#include "main.h"

extern volatile uint32_t Count;
int main(void) {
    BSP_GPIO_InitConfig();
    BSP_CAN_InitConfig();
    BSP_TIM_InitConfig();
    BSP_UART_InitConfig();
    BSP_SPI_InitConfig();
    BSP_I2C_InitConfig();
    BSP_DMA_InitConfig();
    BSP_NVIC_InitConfig();
    BSP_DWT_InitConfig();

    ST7735_Init();
    ST7735_SetOrientation(kRevert);
    ST7735_FillColor(BLACK);
    ST7735_Print(0, 0, GREEN, BLACK, "RM2017 Infantry");
    ST7735_Print(0, 1, GREEN, BLACK, "Gyro Calibrate");

    // SIMULATOR_CameraInit(6);

    CHASSIS_Init();
    ADIS16_Init();
    ADIS16_Calibrate(512);
    MPU6050_Init();

    ST7735_Print(0, 1, GREEN, BLACK, "CNT");
    ST7735_Print(0, 2, GREEN, BLACK, "A_X");
    ST7735_Print(0, 3, GREEN, BLACK, "A_Y");
    ST7735_Print(0, 4, GREEN, BLACK, "A_Z");
    ST7735_Print(0, 5, GREEN, BLACK, "G_X");
    ST7735_Print(0, 6, GREEN, BLACK, "G_Y");
    ST7735_Print(0, 7, GREEN, BLACK, "G_Z");

    BSP_TIM_Start();

    while (1) {
        if (DBUS_Status == kLost) {
            CHASSIS_SetFree();
        }
        ST7735_Print(4, 1, GREEN, BLACK, "%d", Count);
        ST7735_Print(4, 2, GREEN, BLACK, "%d", MPU6050_Data.RawAccel[0]);
        ST7735_Print(4, 3, GREEN, BLACK, "%d", MPU6050_Data.RawAccel[1]);
        ST7735_Print(4, 4, GREEN, BLACK, "%d", MPU6050_Data.RawAccel[2]);
        ST7735_Print(4, 5, GREEN, BLACK, "%d", MPU6050_Data.RawGyro[0]);
        ST7735_Print(4, 6, GREEN, BLACK, "%d", MPU6050_Data.RawGyro[1]);
        ST7735_Print(4, 7, GREEN, BLACK, "%d", MPU6050_Data.RawGyro[2]);
    }
}
