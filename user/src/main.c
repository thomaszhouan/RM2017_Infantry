#include "main.h"

extern volatile int32_t pressCount;
extern volatile uint32_t Count;
extern volatile uint32_t IdleCount;
extern volatile uint32_t FrameLength;
int main(void) {
#if (BOARD_TYPE == BOARD_TYPE_CONTROL)
    CHASSIS_Init();
    GIMBAL_Init();
    GUN_Init();
#endif

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
    ADIS16_Init();
    ADIS16_Calibrate(512);
    // MPU6050_Init();

    ST7735_Print(0, 1, GREEN, BLACK, "pos");
    ST7735_Print(0, 2, GREEN, BLACK, "out");
    ST7735_Print(0, 3, GREEN, BLACK, "tarv");
    ST7735_Print(0, 4, GREEN, BLACK, "vel");
    ST7735_Print(0, 5, GREEN, BLACK, "verr");
#endif

#if (BOARD_TYPE == BOARD_TYPE_JUDGE)
    ST7735_Print(0, 1, GREEN, BLACK, "CNT");
    ST7735_Print(0, 2, GREEN, BLACK, "LEN");
    ST7735_Print(0, 3, GREEN, BLACK, "V");
    ST7735_Print(0, 9, GREEN, BLACK, "RL");
#endif

    BSP_TIM_Start();

    while (1) {
#if (BOARD_TYPE == BOARD_TYPE_CONTROL)
        if (DBUS_Status == kLost) {
            CHASSIS_SetFree();
            GIMBAL_SetFree();
        }
        if (DBUS_Status == kConnected &&
            DBUS_Data.rightSwitchState != kSwitchDown) {
            FRIC_SET_THRUST_L(800);
            FRIC_SET_THRUST_R(800);
            FRIC_SET_THRUST_M(800);
        }
        else {
            FRIC_SET_THRUST_L(800);
            FRIC_SET_THRUST_R(800);
            FRIC_SET_THRUST_M(800);
        }

        ST7735_Print(5, 1, GREEN, BLACK, "%d", GimbalPosition[YAW]);
        ST7735_Print(5, 2, GREEN, BLACK, "%d", GimbalOutput[YAW]);
        ST7735_Print(5, 3, GREEN, BLACK, "%d", GimbalTargetVelocity[YAW]);
        ST7735_Print(5, 4, GREEN, BLACK, "%d", GimbalVelocity[YAW]);
        ST7735_Print(5, 5, GREEN, BLACK, "%d", GimbalTargetVelocity[YAW]-GimbalVelocity[YAW]/2);
#endif

#if (BOARD_TYPE == BOARD_TYPE_JUDGE)
        ST7735_Print(4, 1, GREEN, BLACK, "%d", IdleCount);
        ST7735_Print(4, 2, GREEN, BLACK, "%d", FrameLength);
        ST7735_Print(4, 3, GREEN, BLACK, "%.3f", JUDGE_Data.voltage);
        ST7735_Print(4, 4, GREEN, BLACK, "%.3f", JUDGE_Data.current);
        ST7735_Print(4, 5, GREEN, BLACK, "%d", JUDGE_Data.remainLife);
#endif
    }
}
