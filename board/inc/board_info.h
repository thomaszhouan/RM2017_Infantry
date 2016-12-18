#ifndef BOARD_INFO_H
#define BOARD_INFO_H

typedef enum {
    LED0 = 0,

    LEDn
} Led_TypeDef;

#define LED0_PORT                  GPIOB
#define LED0_PIN                   GPIO_PIN_3
#define LED0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED0_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(_id)  do { if (_id==LED0) LED0_GPIO_CLK_ENABLE();\
                                      } while(0)
#define LEDx_GPIO_CLK_DISABLE(_id) do { if (_id==LED0) LED0_GPIO_CLK_DISABLE();\
                                      } while(0)

#endif // BOARD_INFO_H
