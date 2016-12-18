#include "main.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    HAL_InitTick((1UL << __NVIC_PRIO_BITS) - 1UL);

    LED_Init(LED0);
    BUZZER_Init();
    
    while (1) {
        LED_On(LED0);
        HAL_Delay(500);
        LED_Off(LED0);
        HAL_Delay(500);
    }
}
