#ifndef GPIO_H
#define GPIO_H

typedef void (*GPIO_CallbackTypeDef)(uint16_t pin);

void GPIO_CallbackInstall(uint16_t pin, GPIO_CallbackTypeDef callback);
IRQn_Type GPIO_GetIRQn(uint16_t pin);

#endif // GPIO_H
