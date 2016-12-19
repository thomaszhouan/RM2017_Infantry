#include "main.h"
#include "string.h"

static UART_HandleTypeDef UartHandle;
static uint8_t TxBuffer[] = "Hello there!\n";
#define TXBUFFERSIZE (COUNTOF(TxBuffer)-1)

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

    // Joystick testing
    JOYSTICK_Init(10, 0);
    for (Joystick_TypeDef pos = JUP; pos < JOYSTICKn; ++pos)
        JOYSTICK_CallbackInstall(pos, JOYSTICK_Handler);
    
    while (1) {
        HAL_UART_Transmit(&UartHandle, TxBuffer, TXBUFFERSIZE, 5000);
        LED_On(LED0);
        HAL_Delay(500);
        LED_Off(LED0);
        HAL_Delay(500);
    }
}

void JOYSTICK_Handler(uint16_t GPIO_Pin) {
    static uint8_t msg[][5] = {"0", "1", "13", "14", "15"};

    HAL_Delay(50);
    if (HAL_GPIO_ReadPin(JOYSTICK_PORT, GPIO_Pin)== RESET) {
        switch (GPIO_Pin) {
            case GPIO_PIN_0 : HAL_UART_Transmit(&UartHandle, msg[0], 1, 5000); break;
            case GPIO_PIN_1 : HAL_UART_Transmit(&UartHandle, msg[1], 1, 5000); break;
            case GPIO_PIN_13: HAL_UART_Transmit(&UartHandle, msg[2], 2, 5000); break;
            case GPIO_PIN_14: HAL_UART_Transmit(&UartHandle, msg[3], 2, 5000); break;
            case GPIO_PIN_15: HAL_UART_Transmit(&UartHandle, msg[4], 2, 5000); break;
        }
    }
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}
