#include "common.h"
#include "board_info.h"
#include "joystick.h"

static GPIO_TypeDef* GPIO_PORT[JOYSTICKn] = {JOYSTICK_PORT, JOYSTICK_PORT,
                                             JOYSTICK_PORT, JOYSTICK_PORT,
                                             JOYSTICK_PORT};
static uint16_t GPIO_PIN[JOYSTICKn] = {JOYSTICK_UP_PIN, JOYSTICK_LEFT_PIN,
                                       JOYSTICK_RIGHT_PIN, JOYSTICK_DOWN_PIN,
                                       JOYSTICK_CENTER_PIN};

void JOYSTICK_Init(uint32_t PreemptionPriority, uint32_t SubPriority) {
    JOYSTICK_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    for (uint8_t i = 0; i < JOYSTICKn; ++i) {
        GPIO_InitStruct.Pin = GPIO_PIN[i];
        HAL_GPIO_Init(GPIO_PORT[i], &GPIO_InitStruct);
        IRQn_Type irq = GPIO_GetIRQn(GPIO_PIN[i]);
        HAL_NVIC_SetPriority(irq, PreemptionPriority, SubPriority);
        HAL_NVIC_EnableIRQ(irq);
    }
}

GPIO_PinState JOYSTICK_GetState(Joystick_TypeDef pos) {
    return HAL_GPIO_ReadPin(GPIO_PORT[pos], GPIO_PIN[pos]);
}

void JOYSTICK_CallbackInstall(Joystick_TypeDef pos, GPIO_CallbackTypeDef callback) {
    GPIO_CallbackInstall(GPIO_PIN[pos], callback);
}
