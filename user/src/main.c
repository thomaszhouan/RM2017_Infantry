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
    BSP_ADC_InitConfig();
    BSP_NVIC_InitConfig();
    BSP_DWT_InitConfig();

    ST7735_Init();
    ST7735_SetOrientation(kRevert);
    ST7735_FillColor(BLACK);
    ST7735_Print(0, 0, GREEN, BLACK, "RM2017 Infantry");
    ST7735_Print(0, 1, GREEN, BLACK, "Gyro Calibrate");

#if (BOARD_TYPE == BOARD_TYPE_JUDGE)
    FLASH_Load();
    SIMULATOR_Init();
    SIMULATOR_CameraInit(7);
#endif

#if (BOARD_TYPE == BOARD_TYPE_CONTROL)
    CHASSIS_Init();
    GIMBAL_Init();
    GUN_Init();
    ADIS16_Init();
    ADIS16_Calibrate(512);
    // MPU6050_Init();

    ST7735_Print(0, 1, GREEN, BLACK, "RL");
    ST7735_Print(0, 2, GREEN, BLACK, "V");
    ST7735_Print(0, 3, GREEN, BLACK, "I");
    ST7735_Print(0, 4, GREEN, BLACK, "pos");
    ST7735_Print(0, 5, GREEN, BLACK, "vel");
    ST7735_Print(0, 6, GREEN, BLACK, "enc");
    ST7735_Print(0, 7, GREEN, BLACK, "out");
    ST7735_Print(0, 8, GREEN, BLACK, "tar");
#endif

#if (BOARD_TYPE == BOARD_TYPE_JUDGE)
    ST7735_Print(0, 1, GREEN, BLACK, "RL");
    ST7735_Print(0, 2, GREEN, BLACK, "PR");
    ST7735_Print(0, 3, GREEN, BLACK, "I");
    ST7735_Print(0, 4, GREEN, BLACK, "V");
    ST7735_Print(0, 5, GREEN, BLACK, "WR");
#endif

    BSP_TIM_Start();

    while (1) {
#if (BOARD_TYPE == BOARD_TYPE_CONTROL)
        if (DBUS_Status == kLost) {
            CHASSIS_SetFree();
        }
        if (DBUS_Status == kConnected &&
            DBUS_Data.leftSwitchState == kSwitchMiddle) {
            FRIC_SET_THRUST_L(1000);
            FRIC_SET_THRUST_R(1000);
        }
        else {
            FRIC_SET_THRUST_L(0);
            FRIC_SET_THRUST_R(0);
        }

        ST7735_Print(4, 1, GREEN, BLACK, "%d", JUDGE_Data.remainLife);
        ST7735_Print(4, 2, GREEN, BLACK, "%.2f", JUDGE_Data.voltage);
        ST7735_Print(4, 3, GREEN, BLACK, "%.2f", JUDGE_Data.current);
        ST7735_Print(4, 4, GREEN, BLACK, "%d", GimbalPosition[0]);
        ST7735_Print(4, 5, GREEN, BLACK, "%d", GimbalVelocity[0]);
        ST7735_Print(4, 6, GREEN, BLACK, "%d", ENCODER_Data);
        ST7735_Print(4, 7, GREEN, BLACK, "%d", GUN_Data.pokeOutput);
        ST7735_Print(4, 8, GREEN, BLACK, "%d", GUN_Data.pokeTargetSpeed);
#endif

#if (BOARD_TYPE == BOARD_TYPE_JUDGE)
        MENU_CheckJS();
        ST7735_Print(4, 1, GREEN, BLACK, "%d", SIMULATOR_Data.remainLife);
        ST7735_Print(4, 2, GREEN, BLACK, "%.5f", SIMULATOR_Data.power);
        ST7735_Print(4, 3, GREEN, BLACK, "%.5f", SIMULATOR_Data.current);
        ST7735_Print(4, 4, GREEN, BLACK, "%.5f", SIMULATOR_Data.voltage);
        ST7735_Print(4, 5, GREEN, BLACK, "%.5f", SIMULATOR_Data.remainEnergy);
#endif
    }
}
