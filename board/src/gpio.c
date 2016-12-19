#include "common.h"
#include "board_info.h"
#include "gpio.h"

#define GPIO_NUMBER           ((uint32_t)16U)

static GPIO_CallbackTypeDef CallbackTable[GPIO_NUMBER] = {NULL};

// private utility function
static uint16_t GET_INDEX(uint16_t pin) {
    assert_param(IS_GPIO_PIN(pin));

    uint16_t index = 0;
    while (!(pin&((uint16_t)1U))) {
        pin >>= 1;
        ++index;
    }
    return index;
}

void GPIO_CallbackInstall(uint16_t pin, GPIO_CallbackTypeDef callback) {
    uint16_t index = GET_INDEX(pin);
    CallbackTable[index] = callback;
}

IRQn_Type GPIO_GetIRQn(uint16_t pin) {
    switch (pin) {
        case GPIO_PIN_0: return EXTI0_IRQn;
        case GPIO_PIN_1: return EXTI1_IRQn;
        case GPIO_PIN_2: return EXTI2_IRQn;
        case GPIO_PIN_3: return EXTI3_IRQn;
        case GPIO_PIN_4: return EXTI4_IRQn;
        case GPIO_PIN_5: case GPIO_PIN_6: case GPIO_PIN_7:
        case GPIO_PIN_8: case GPIO_PIN_9:
        return EXTI9_5_IRQn;
        case GPIO_PIN_10: case GPIO_PIN_11: case GPIO_PIN_12:
        case GPIO_PIN_13: case GPIO_PIN_14: case GPIO_PIN_15:
        return EXTI15_10_IRQn;
        default: return EXTI0_IRQn;
    }
}

/*
    This function overrides the EXTI callback in stm32f4xx_hal_gpio.h
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    uint16_t index = GET_INDEX(GPIO_Pin);
    assert_param(index < GPIO_NUMBER);
    if (CallbackTable[index])
        CallbackTable[index](GPIO_Pin);
}
