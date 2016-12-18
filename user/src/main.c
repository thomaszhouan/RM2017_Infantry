#include "main.h"

static void Delay(uint32_t cnt) {
    while (--cnt)
        ;
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    LED_Init(LED0);

    while (1) {
        LED_On(LED0);
        Delay(0x7FFFFF);
        LED_Off(LED0);
        Delay(0x7FFFFF);
    }
}
