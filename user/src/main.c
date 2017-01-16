#include "main.h"
#include <stdio.h>
#include <string.h>

void JOYSTICK_Handler(uint16_t GPIO_Pin);
void Error_Handler(void);

char message[256];

int main(void) {
    /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
    HAL_Init();
    SystemClock_Config();
    HAL_NVIC_SetPriority(SysTick_IRQn, 5, 0);
    DWT_DelayInit();
    HAL_Delay(200);

    // LCD
    ST7735_Init();
    ST7735_SetOrientation(kRevert);
    ST7735_FillColor(BLACK);
    ST7735_Print(0, 0, GREEN, BLACK, "Gyro Calibrate");

#if (USE_GYRO==1)
    ADIS16_Init();
    ADIS16_Calibrate(512);
#endif // USE_GYRO

    LED_Init(LED0);
    BUZZER_Init();
    DBUS_Init();
    JUDGE_Init();
    CHASSIS_Init();
    GIMBAL_Init();
    MONITOR_Init();
    MPU6050_Init();
    
    JOYSTICK_Init(15, 0);
    for (Joystick_TypeDef pos = JUP; pos < JOYSTICKn; ++pos)
        JOYSTICK_CallbackInstall(pos, JOYSTICK_Handler);

    ST7735_Print(0, 0, GREEN, BLACK, "RM2017 Infantry");
    ST7735_Print(0, 1, GREEN, BLACK, "Pos");
    ST7735_Print(0, 2, GREEN, BLACK, "RV");
    
    // TIM2 init (1ms)
    __HAL_RCC_TIM2_CLK_ENABLE();
    Tim2_Handle.Instance = TIM2;
    Tim2_Handle.Init.Prescaler = (uint32_t) (((SystemCoreClock / 2) / 10000)-1);
    Tim2_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    Tim2_Handle.Init.Period = 10-1;
    Tim2_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&Tim2_Handle);

    HAL_NVIC_SetPriority(TIM2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

#if (USE_GYRO==1)
    // TI3 init (256Hz) [256 * 125 = 32000]
    __HAL_RCC_TIM3_CLK_ENABLE();
    Tim3_Handle.Instance = TIM3;
    Tim3_Handle.Init.Prescaler = (uint32_t) (((SystemCoreClock / 2) / 32000)-1);
    Tim3_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    Tim3_Handle.Init.Period = 125-1;
    Tim3_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&Tim3_Handle);

    HAL_NVIC_SetPriority(TIM3_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    HAL_TIM_Base_Start_IT(&Tim3_Handle);
#endif // USE_GYRO
    
    // start TIM2
    HAL_TIM_Base_Start_IT(&Tim2_Handle);
    while (1) {
        if (DBUS_Status == kConnected) {

        }
        else { // DBUS connection lost
            CHASSIS_SetFree();
            GIMBAL_SetFree();
        }
        ST7735_Print(4, 1, GREEN, BLACK, "%d", GimbalPosition[0]);
        ST7735_Print(4, 2, GREEN, BLACK, "%d", GimbalVelocity[0]);
        sprintf(message, "%d %d %d\n",
            MPU6050_GyroData[0], MPU6050_GyroData[1], MPU6050_GyroData[2]);
        MONITOR_Send((uint8_t*)message, strlen(message));
    }
}

void JOYSTICK_Handler(uint16_t GPIO_Pin) {
    static uint8_t msg[JOYSTICKn][10] = {"up\n", "left\n", "right\n", "down\n", "center\n"};
    UNUSED(msg);

    // Prevent spike
    HAL_Delay(50);
    if (HAL_GPIO_ReadPin(JOYSTICK_PORT, GPIO_Pin)== RESET) {
        switch (GPIO_Pin) {
            case JOYSTICK_UP_PIN:
            break;
            case JOYSTICK_LEFT_PIN:
            break;
            case JOYSTICK_RIGHT_PIN:
            break;
            case JOYSTICK_DOWN_PIN:
            break;
            case JOYSTICK_CENTER_PIN:
            break;
        }
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle) {
    if (handle == &DBUS_UART_HANDLE) {
        DBUS_Decode();
        CHASSIS_SetMotion();
        GIMBAL_SetMotion();
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle) {
    UNUSED(handle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle) {
    static uint32_t tick = 0;
    UNUSED(handle);

    ++tick;
    if (tick == 1000) {
        tick = 0;
    }
    if (tick % 500 == 0) {
        LED_Toggle(LED0);
    }

    // check DBUS connection
    if (tick % 20 == 0) {
        DBUS_UpdateStatus();
    }

    if (tick % 5 == 1)
        MPU6050_ReadGyroData();

    if (DBUS_Status == kConnected) {
        CHASSIS_Control();
        // GIMBAL_Control();
    }
    CHASSIS_SendCmd();
    // GIMBAL_SendCmd();
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *handle) {
    if (handle == &CHASSIS_CAN_HANDLE) {
        switch (CHASSIS_CAN_RX.StdId) {
            case FL_MOTOR_ID: case FR_MOTOR_ID:
            case BR_MOTOR_ID: case BL_MOTOR_ID: {
                CHASSIS_UpdateMeasure(CHASSIS_CAN_RX.StdId);
            } break;
        }

        // enable interrupt directly to speed up
        __HAL_CAN_ENABLE_IT(handle, CAN_IT_FMP0);
    }
    else { // handle == &GIMBAL_CAN_HANDLE
        switch (GIMBAL_CAN_RX.StdId) {
            case GIMBAL_YAW_ID: case GIMBAL_PITCH_ID: {
                GIMBAL_UpdateMeasure(GIMBAL_CAN_RX.StdId);
            } break;
        }
        __HAL_CAN_ENABLE_IT(handle, CAN_IT_FMP0);
    }
}

void Error_Handler(void) {
    BUZZER_On();
    while (1)
        ;
}
