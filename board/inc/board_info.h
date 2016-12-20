#ifndef BOARD_INFO_H
#define BOARD_INFO_H

/* external handles */
// USART1
#define USE_USART1
extern UART_HandleTypeDef Uart1_Handle;
extern DMA_HandleTypeDef Uart1_TxDmaHandle, Uart1_RxDmaHandle;

// USART3
#define USE_USART3
extern UART_HandleTypeDef Uart3_Handle;
extern DMA_HandleTypeDef Uart3_TxDmaHandle, Uart3_RxDmaHandle;

// TIM2
#define USE_TIM2
extern TIM_HandleTypeDef Tim2_Handle;

typedef enum {
    LED0 = 0,

    LEDn
} Led_TypeDef;

#define LED0_PORT                               GPIOB
#define LED0_PIN                                GPIO_PIN_3
#define LED0_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED0_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(_id)               do { if (_id==LED0) LED0_GPIO_CLK_ENABLE();\
                                                   } while(0)
#define LEDx_GPIO_CLK_DISABLE(_id)              do { if (_id==LED0) LED0_GPIO_CLK_DISABLE();\
                                                   } while(0)

#define BUZZER_PORT                             GPIOB
#define BUZZER_PIN                              GPIO_PIN_1
#define BUZZER_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUZZER_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOB_CLK_DISABLE()

typedef enum {
    UART1 = 0,
    UART3,

    UARTn
} Uart_TypeDef;

#define UART1_PORT                              GPIOB
#define UART1_TX_PIN                            GPIO_PIN_6
#define UART1_RX_PIN                            GPIO_PIN_7
#define UART1_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
#define UART1_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOB_CLK_DISABLE()
#define UART1_CLK_ENABLE()                      __HAL_RCC_USART1_CLK_ENABLE()
#define UART1_CLK_DISABLE()                     __HAL_RCC_USART1_CLK_DISABLE()
#define UART1_DMA_CLK_ENABLE()                  __HAL_RCC_DMA2_CLK_ENABLE()
#define UART1_DMA_CLK_DISABLE()                 __HAL_RCC_DMA2_CLK_DISABLE()

#define UART3_PORT                              GPIOB
#define UART3_TX_PIN                            GPIO_PIN_10
#define UART3_RX_PIN                            GPIO_PIN_11
#define UART3_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOB_CLK_ENABLE()
#define UART3_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOB_CLK_DISABLE()
#define UART3_CLK_ENABLE()                      __HAL_RCC_USART3_CLK_ENABLE()
#define UART3_CLK_DISABLE()                     __HAL_RCC_USART3_CLK_DISABLE()
#define UART3_DMA_CLK_ENABLE()                  __HAL_RCC_DMA1_CLK_ENABLE()
#define UART3_DMA_CLK_DISABLE()                 __HAL_RCC_DMA1_CLK_DISABLE()

#define UARTx_GPIO_CLK_ENABLE(_id)              do { if (_id==UART1) UART1_GPIO_CLK_ENABLE();\
                                                     else if (_id==UART3) UART3_GPIO_CLK_ENABLE();\
                                                   } while(0)
#define UARTx_GPIO_CLK_DISABLE(_id)             do { if (_id==UART1) UART1_GPIO_CLK_DISABLE();\
                                                     else if (_id==UART3) UART3_GPIO_CLK_DISABLE();\
                                                   } while(0)
#define UARTx_CLK_ENABLE(_id)                   do { if (_id==UART1) UART1_CLK_ENABLE();\
                                                     else if (_id==UART3) UART3_CLK_ENABLE();\
                                                   } while(0)
#define UARTx_CLK_DISABLE(_id)                  do { if (_id==UART1) UART1_CLK_DISABLE();\
                                                     else if (_id==UART3) UART3_CLK_DISABLE();\
                                                   } while(0)
#define UARTx_DMA_CLK_ENABLE(_id)               do { if (_id==UART1) UART1_DMA_CLK_ENABLE();\
                                                     else if (_id==UART3) UART3_DMA_CLK_ENABLE();\
                                                   } while(0)
#define UARTx_DMA_CLK_DISABLE(_id)              do { if (_id==UART1) UART1_DMA_CLK_DISABLE();\
                                                     else if (_id==UART3) UART3_DMA_CLK_DISABLE();\
                                                   } while(0)

typedef enum {
    JUP = 0,
    JLEFT,
    JRIGHT,
    JDOWN,
    JCENTER,

    JOYSTICKn
} Joystick_TypeDef;

#define JOYSTICK_PORT                           GPIOC
#define JOYSTICK_UP_PIN                         GPIO_PIN_15
#define JOYSTICK_LEFT_PIN                       GPIO_PIN_0
#define JOYSTICK_RIGHT_PIN                      GPIO_PIN_14
#define JOYSTICK_DOWN_PIN                       GPIO_PIN_1
#define JOYSTICK_CENTER_PIN                     GPIO_PIN_13
#define JOYSTICK_ALL_PIN                        (JOYSTICK_UP_PIN | JOYSTICK_LEFT_PIN |\
                                                 JOYSTICK_RIGHT_PIN | JOYSTICK_DOWN_PIN |\
                                                 JOYSTICK_CENTER_PIN)
#define JOYSTICK_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOC_CLK_ENABLE()
#define JOYSTICK_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOC_CLK_DISABLE()

#endif // BOARD_INFO_H
