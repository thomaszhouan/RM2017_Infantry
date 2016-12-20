#include "main.h"

UART_HandleTypeDef *UartHandle = &Uart3_Handle;
static uint8_t TxBuffer[] = "Hello DMA\n";
#define TXBUFFERSIZE (COUNTOF(TxBuffer)-1)
#define RXBUFFERSIZE 128U
static uint8_t RxBuffer[RXBUFFERSIZE];

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
    UART_InitStruct.PreemptionPriority     = 12;
    UART_InitStruct.SubPriority            = 0;
    UART_InitStruct.DMA_Tx_Mode            = DMA_NORMAL;
    UART_InitStruct.DMA_Rx_Mode            = DMA_NORMAL;
    UART_InitStruct.DMA_PreemptionPriority = 8;
    UART_InitStruct.DMA_SubPriority        = 0;
    UART_Init(&UART_InitStruct);
    UNUSED(TxBuffer);
    UNUSED(RxBuffer);

    JOYSTICK_Init(10, 0);
    for (Joystick_TypeDef pos = JUP; pos < JOYSTICKn; ++pos)
        JOYSTICK_CallbackInstall(pos, JOYSTICK_Handler);

    ST7735_Init();
    ST7735_SetOrientation(kRevert);
    ST7735_FillColor(BLACK);
    ST7735_Print(0, 0, GREEN, BLACK, "RM2017 Thomas");
    ST7735_Print(0, 1, GREEN, BLACK, "CH1");
    ST7735_Print(0, 2, GREEN, BLACK, "CH2");
    ST7735_Print(0, 3, GREEN, BLACK, "CH3");
    ST7735_Print(0, 4, GREEN, BLACK, "CH4");
    ST7735_Print(0, 5, GREEN, BLACK, "X");
    ST7735_Print(0, 6, GREEN, BLACK, "Y");
    ST7735_Print(0, 7, GREEN, BLACK, "Z");

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
    HAL_TIM_Base_Start_IT(&Tim2_Handle);
    
    uint32_t DBUS_LastCount = DBUS_FrameCount;
    while (1) {
        if (DBUS_FrameCount != DBUS_LastCount) {
            DBUS_LastCount = DBUS_FrameCount;
            ST7735_Print(4, 1, GREEN, BLACK, "%d", DBUS_Data.ch1);
            ST7735_Print(4, 2, GREEN, BLACK, "%d", DBUS_Data.ch2);
            ST7735_Print(4, 3, GREEN, BLACK, "%d", DBUS_Data.ch3);
            ST7735_Print(4, 4, GREEN, BLACK, "%d", DBUS_Data.ch4);
            ST7735_Print(4, 5, GREEN, BLACK, "%d", DBUS_Data.mouse.x);
            ST7735_Print(4, 6, GREEN, BLACK, "%d", DBUS_Data.mouse.y);
            ST7735_Print(4, 7, GREEN, BLACK, "%d", DBUS_Data.mouse.z);
        }
    }
}

void JOYSTICK_Handler(uint16_t GPIO_Pin) {
    static uint8_t msg[JOYSTICKn][10] = {"up\n", "left\n", "right\n", "down\n", "center\n"};

    // Prevent spike
    HAL_Delay(50);
    if (HAL_GPIO_ReadPin(JOYSTICK_PORT, GPIO_Pin)== RESET) {
        switch (GPIO_Pin) {
            case JOYSTICK_UP_PIN    : HAL_UART_Transmit_IT(UartHandle, msg[JUP], Strlen(msg[JUP])); break;
            case JOYSTICK_LEFT_PIN  : HAL_UART_Transmit(UartHandle, msg[JLEFT], Strlen(msg[JLEFT]), 5000); break;
            case JOYSTICK_RIGHT_PIN : HAL_UART_Transmit(UartHandle, msg[JRIGHT], Strlen(msg[JRIGHT]), 5000); break;
            case JOYSTICK_DOWN_PIN  : HAL_UART_Transmit(UartHandle, msg[JDOWN], Strlen(msg[JDOWN]), 5000); break;
            case JOYSTICK_CENTER_PIN: HAL_UART_Transmit(UartHandle, msg[JCENTER], Strlen(msg[JCENTER]), 5000); break;
        }
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle) {
    if (handle == &DBUS_UART_HANDLE) {
        DBUS_Decode();
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
}

void Error_Handler(void) {
    BUZZER_On();
    while (1)
        ;
}
