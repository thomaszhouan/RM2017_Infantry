#include "main.h"
#include <stdio.h>
#include <string.h>

void JOYSTICK_Handler(uint16_t GPIO_Pin);
void Error_Handler(void);

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

    // LCD
    ST7735_Init();
    ST7735_SetOrientation(kRevert);
    ST7735_FillColor(BLACK);
    ST7735_Print(0, 0, GREEN, BLACK, "Gyro Calibrate");

    LED_Init(LED0);
    BUZZER_Init();
    DBUS_Init();
    JUDGE_Init();
    CHASSIS_Init();
    ADIS16_Init();
    ADIS16_Calibrate(512);

    JOYSTICK_Init(15, 0);
    for (Joystick_TypeDef pos = JUP; pos < JOYSTICKn; ++pos)
        JOYSTICK_CallbackInstall(pos, JOYSTICK_Handler);

    ST7735_Print(0, 0, GREEN, BLACK, "RM2017 Infantry");
    ST7735_Print(0, 1, GREEN, BLACK, "Volt");
    ST7735_Print(0, 2, GREEN, BLACK, "Curr");
    ST7735_Print(0, 3, GREEN, BLACK, "RL");
    ST7735_Print(0, 4, GREEN, BLACK, "Omg");
    ST7735_Print(0, 5, GREEN, BLACK, "The");
    
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
    
    // start TIM2
    HAL_TIM_Base_Start_IT(&Tim2_Handle);
    while (1) {
        if (DBUS_Status == kConnected) {

        }
        else { // DBUS connection lost
            CHASSIS_SetFree();
        }
        ST7735_Print(5, 1, GREEN, BLACK, "%.2f", JUDGE_Data.voltage);
        ST7735_Print(5, 2, GREEN, BLACK, "%.2f", JUDGE_Data.current);
        ST7735_Print(5, 3, GREEN, BLACK, "%d", JUDGE_Data.remainLife);
        ST7735_Print(5, 4, GREEN, BLACK, "%d", ADIS16_Data.omega);
        ST7735_Print(5, 5, GREEN, BLACK, "%.2f", ADIS16_Data.theta);
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
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *handle) {
    UNUSED(handle);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle) {
    static uint32_t tick = 0;
    UNUSED(handle);

    ++tick;
    if (tick == 1000) tick = 0;
    if (tick % 500 == 0) {
        LED_Toggle(LED0);
    }

    if (tick % 4 == 0)
        ADIS16_Update();

    // check DBUS connection
    if (tick % 20 == 0) {
        DBUS_UpdateStatus();
    }

    if (DBUS_Status == kConnected) {
        CHASSIS_Control();
    }
    CHASSIS_SendCmd();
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
}

void Error_Handler(void) {
    BUZZER_On();
    while (1)
        ;
}
