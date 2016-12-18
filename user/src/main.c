#include "main.h"

static UART_HandleTypeDef UartHandle;
static uint8_t TxBuffer[] = "Hello there!\n";
#define TXBUFFERSIZE (COUNTOF(TxBuffer)-1)

int main(void) {
    HAL_Init();
    SystemClock_Config();
    HAL_InitTick((1UL << __NVIC_PRIO_BITS) - 1UL);

    LED_Init(LED0);
    BUZZER_Init();
    UART_Init(&UartHandle, UART1, 115200);
    
    while (1) {
        HAL_UART_Transmit(&UartHandle, TxBuffer, TXBUFFERSIZE, 5000);
        LED_On(LED0);
        HAL_Delay(500);
        LED_Off(LED0);
        HAL_Delay(500);
    }
}
