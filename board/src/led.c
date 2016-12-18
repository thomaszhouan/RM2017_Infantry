#include "common.h"
#include "board_info.h"
#include "led.h"

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED0_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED0_PIN};

void LED_Init(Led_TypeDef led) {
    LEDx_GPIO_CLK_ENABLE(led);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN[led];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIO_PORT[led], &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIO_PORT[led], GPIO_PIN[led], GPIO_PIN_SET);
}

void LED_On(Led_TypeDef led) {
    HAL_GPIO_WritePin(GPIO_PORT[led], GPIO_PIN[led], GPIO_PIN_RESET);
}

void LED_Off(Led_TypeDef led) {
    HAL_GPIO_WritePin(GPIO_PORT[led], GPIO_PIN[led], GPIO_PIN_SET);
}

void LED_Toggle(Led_TypeDef led) {
    HAL_GPIO_TogglePin(GPIO_PORT[led], GPIO_PIN[led]);
}
