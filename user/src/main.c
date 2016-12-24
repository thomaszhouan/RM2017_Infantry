#include "main.h"
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef *UartHandle = &Uart3_Handle;

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

    LED_Init(LED0);
    BUZZER_Init();
    DBUS_Init();

    // USART3 init
    UART_SimpleInitTypeDef UART_InitStruct;
    UART_InitStruct.Instance               = UART3;
    UART_InitStruct.UartHandle             = &Uart3_Handle;
    UART_InitStruct.DmaHandleTx            = &Uart3_TxDmaHandle;
    UART_InitStruct.DmaHandleRx            = &Uart3_RxDmaHandle;
    UART_InitStruct.Baudrate               = 115200;
    UART_InitStruct.Parity                 = UART_PARITY_NONE;
    UART_InitStruct.PreemptionPriority     = 13;
    UART_InitStruct.SubPriority            = 0;
    UART_InitStruct.DMA_Tx_Mode            = DMA_NORMAL;
    UART_InitStruct.DMA_Rx_Mode            = DMA_NORMAL;
    UART_InitStruct.DMA_PreemptionPriority = 13;
    UART_InitStruct.DMA_SubPriority        = 0;
    UART_Init(&UART_InitStruct);

    JOYSTICK_Init(15, 0);
    for (Joystick_TypeDef pos = JUP; pos < JOYSTICKn; ++pos)
        JOYSTICK_CallbackInstall(pos, JOYSTICK_Handler);

    // LCD
    ST7735_Init();
    ST7735_SetOrientation(kRevert);
    ST7735_FillColor(BLACK);
    ST7735_Print(0, 0, GREEN, BLACK, "RM2017 Infantry");
    ST7735_Print(0, 1, GREEN, BLACK, "CH2");
    ST7735_Print(0, 2, GREEN, BLACK, "V");
    ST7735_Print(0, 3, GREEN, BLACK, "TAR");
    ST7735_Print(0, 4, GREEN, BLACK, "OUT");

    // CAN1
    CAN_SimpleInitTypeDef CAN_InitStruct;
    CAN_InitStruct.id = CHASSIS_CAN_ID;
    CAN_InitStruct.CanHandle = &CHASSIS_CAN_HANDLE;
    CAN_InitStruct.CanRx = &CHASSIS_CAN_RX;
    CAN_InitStruct.CanTx = &CHASSIS_CAN_TX;
    CAN_InitStruct.PreemptionPriority = 9;
    CAN_InitStruct.SubPriority = 0;
    CAN_Init(&CAN_InitStruct);

    // CAN Tx
    CHASSIS_CAN_TX.StdId = 0x200;
    CHASSIS_CAN_TX.IDE = CAN_ID_STD;
    CHASSIS_CAN_TX.RTR = CAN_RTR_DATA;
    CHASSIS_CAN_TX.DLC = 8;
    
    // TIM init
    __HAL_RCC_TIM2_CLK_ENABLE();
    Tim2_Handle.Instance = TIM2;
    Tim2_Handle.Init.Prescaler = (uint32_t) (((SystemCoreClock / 2) / 10000)-1);
    Tim2_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    Tim2_Handle.Init.Period = 10-1;
    Tim2_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&Tim2_Handle);

    HAL_NVIC_SetPriority(TIM2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    CHASSIS_Init();
    HAL_CAN_Receive_IT(&CHASSIS_CAN_HANDLE, 0);
    
    HAL_TIM_Base_Start_IT(&Tim2_Handle);
    while (1) {
        if (DBUS_Status == kConnected) {
            ST7735_Print(4, 1, GREEN, BLACK, "%d", DBUS_Data.ch2);
        }
        else { // DBUS connection lost
            ST7735_Print(4, 1, GREEN, BLACK, "N/A");
            CHASSIS_SetFree();
        }
        ST7735_Print(4, 2, GREEN, BLACK, "%d", MotorVelocity[0]);
        ST7735_Print(4, 3, GREEN, BLACK, "%d", TargetVelocity[0]);
        ST7735_Print(4, 4, GREEN, BLACK, "%d", MotorOutput[0]);
    }
}

void JOYSTICK_Handler(uint16_t GPIO_Pin) {
    static uint8_t msg[JOYSTICKn][10] = {"up\n", "left\n", "right\n", "down\n", "center\n"};

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
            HAL_UART_Transmit_IT(UartHandle, msg[JCENTER], Strlen(msg[JCENTER]));
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
    if (tick % 500 == 0)
        LED_Toggle(LED0);

    // check DBUS connection
    if (tick % 20 == 0) {
        DBUS_UpdateStatus();
    }

    if (DBUS_Status == kConnected) {
        for (uint16_t id = 0x201; id <= 0x204; ++id)
            CHASSIS_MotorControl(id);
    }
    CHASSIS_SendCmd();
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *handle) {
    if (handle == &CHASSIS_CAN_HANDLE) {
        CHASSIS_UpdateMeasure(CHASSIS_CAN_RX.StdId);

        // enable interrupt directly to speed up
        __HAL_CAN_ENABLE_IT(handle, CAN_IT_FMP0);
    }
}

void Error_Handler(void) {
    BUZZER_On();
    while (1)
        ;
}
