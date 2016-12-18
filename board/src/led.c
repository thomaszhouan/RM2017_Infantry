#include "common.h"
#include "board_info.h"
#include "led.h"

void LED_Init(uint8_t id) {
    GPIO_TypeDef *GPIOx;
    uint32_t pin;

    switch (id) {
        case LED0: GPIOx = LED0_PORT; pin = LED0_PIN; break;
    }

    if (GPIOx == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void LED_On(uint8_t id) {
    switch (id) {
        case LED0: HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_RESET); break;
    }
}

void LED_Off(uint8_t id) {
    switch (id) {
        case LED0: HAL_GPIO_WritePin(LED0_PORT, LED0_PIN, GPIO_PIN_SET); break;
    }
}

void LED_Toggle(uint8_t id) {
    switch (id) {
        case LED0: HAL_GPIO_TogglePin(LED0_PORT, LED0_PIN); break;
    }
}
