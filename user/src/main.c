#include "main.h"

UART_HandleTypeDef UartHandle;
#define RXBUFFERSIZE (128U)
static uint8_t RxBuffer[RXBUFFERSIZE];

TIM_HandleTypeDef TimHandle;

void JOYSTICK_Handler(uint16_t GPIO_Pin);

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
    UART_Init(&UartHandle, UART1, 115200);
    HAL_NVIC_SetPriority(USART1_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    JOYSTICK_Init(10, 0);
    for (Joystick_TypeDef pos = JUP; pos < JOYSTICKn; ++pos)
        JOYSTICK_CallbackInstall(pos, JOYSTICK_Handler);

    // TIM init
    __HAL_RCC_TIM2_CLK_ENABLE();
    TimHandle.Instance = TIM2;
    TimHandle.Init.Prescaler = (uint32_t) (((SystemCoreClock / 2) / 10000)-1);
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.Period = 10-1;
    TimHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TimHandle);

    HAL_NVIC_SetPriority(TIM2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    HAL_TIM_Base_Start_IT(&TimHandle);
    
    while (1) {

    }
}

void JOYSTICK_Handler(uint16_t GPIO_Pin) {
    static uint8_t msg[JOYSTICKn][10] = {"up\n", "left\n", "right\n", "down\n", "center\n"};

    // Prevent spike
    HAL_Delay(50);
    if (HAL_GPIO_ReadPin(JOYSTICK_PORT, GPIO_Pin)== RESET) {
        switch (GPIO_Pin) {
            case JOYSTICK_UP_PIN    : HAL_UART_Transmit(&UartHandle, msg[JUP], Strlen(msg[JUP]), 5000); break;
            case JOYSTICK_LEFT_PIN  : HAL_UART_Transmit(&UartHandle, msg[JLEFT], Strlen(msg[JLEFT]), 5000); break;
            case JOYSTICK_RIGHT_PIN : HAL_UART_Transmit(&UartHandle, msg[JRIGHT], Strlen(msg[JRIGHT]), 5000); break;
            case JOYSTICK_DOWN_PIN  : HAL_UART_Transmit(&UartHandle, msg[JDOWN], Strlen(msg[JDOWN]), 5000); break;
            case JOYSTICK_CENTER_PIN: HAL_UART_Transmit(&UartHandle, msg[JCENTER], Strlen(msg[JCENTER]), 5000); break;
        }
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle) {
    UNUSED(handle);
    BUZZER_Beep();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle) {
    static uint32_t tick = 0;
    ++tick;
    if (tick == 1000) tick = 0;
    UNUSED(handle);
    if (tick % 500 == 0)
        LED_Toggle(LED0);
}
