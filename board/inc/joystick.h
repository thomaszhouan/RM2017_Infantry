#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "gpio.h"

void JOYSTICK_Init(uint32_t PreemptionPriority, uint32_t SubPriority);
GPIO_PinState JOYSTICK_GetState(Joystick_TypeDef pos);
void JOYSTICK_CallbackInstall(Joystick_TypeDef pos, GPIO_CallbackTypeDef callback);

#endif // JOYSTICK_H
