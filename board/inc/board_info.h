#ifndef BOARD_INFO_H
#define BOARD_INFO_H

/*----------External Handles----------*/
#ifndef HANDLE_FILE
    #define HANDLE_EXT extern
#else
    #define HANDLE_EXT
#endif
// USART1
#define USE_USART1
HANDLE_EXT UART_HandleTypeDef Uart1_Handle;
HANDLE_EXT DMA_HandleTypeDef Uart1_TxDmaHandle, Uart1_RxDmaHandle;

// USART3
#define USE_USART3
HANDLE_EXT UART_HandleTypeDef Uart3_Handle;
HANDLE_EXT DMA_HandleTypeDef Uart3_TxDmaHandle, Uart3_RxDmaHandle;

// TIM2
#define USE_TIM2
HANDLE_EXT TIM_HandleTypeDef Tim2_Handle;

// SPI1
#define USE_SPI1
HANDLE_EXT SPI_HandleTypeDef Spi1_Handle;
#define LCD_SPI_HANDLE Spi1_Handle

// CAN1
#define USE_CAN1
HANDLE_EXT CAN_HandleTypeDef Can1_Handle;
HANDLE_EXT CanRxMsgTypeDef Can1_RxMsg;
HANDLE_EXT CanTxMsgTypeDef Can1_TxMsg;

// CAN2
#define USE_CAN2
HANDLE_EXT CAN_HandleTypeDef Can2_Handle;
HANDLE_EXT CanRxMsgTypeDef Can2_RxMsg;
HANDLE_EXT CanTxMsgTypeDef Can2_TxMsg;

/*----------LED----------*/
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

/*----------Buzzer----------*/
#define BUZZER_PORT                             GPIOB
#define BUZZER_PIN                              GPIO_PIN_1
#define BUZZER_GPIO_CLK_ENABLE()                __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUZZER_GPIO_CLK_DISABLE()               __HAL_RCC_GPIOB_CLK_DISABLE()

/*----------UART----------*/
typedef enum {
    UART1 = 0,
    UART3,

    UARTn
} Uart_TypeDef;

#define DBUS_UART_NUM                           1

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

/*----------Joystick----------*/
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

/*----------LCD----------*/
#define LCD_GPIO_CLK_ENABLE()                   do { __HAL_RCC_GPIOA_CLK_ENABLE();\
                                                     __HAL_RCC_GPIOB_CLK_ENABLE();\
                                                     __HAL_RCC_GPIOC_CLK_ENABLE();\
                                                   } while(0)
#define LCD_SPI_CLK_ENABLE()                    __HAL_RCC_SPI1_CLK_ENABLE()
#define LCD_CS_PORT                             GPIOC
#define LCD_DC_PORT                             GPIOC
#define LCD_RST_PORT                            GPIOB
#define LCD_SCLK_PORT                           GPIOA
#define LCD_SDAT_PORT                           GPIOA
#define LCD_CS_PIN                              GPIO_PIN_4
#define LCD_DC_PIN                              GPIO_PIN_5
#define LCD_RST_PIN                             GPIO_PIN_0
#define LCD_SCLK_PIN                            GPIO_PIN_5
#define LCD_SDAT_PIN                            GPIO_PIN_7

/*----------CAN----------*/
typedef enum {
    xCAN1 = 0,
    xCAN2,

    xCANn
} xCAN_TypeDef;

#define CAN1_GPIO_PORT                          GPIOA
#define CAN1_GPIO_PIN                           (GPIO_PIN_11 | GPIO_PIN_12)
#define CAN1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()

#define CAN2_GPIO_PORT                          GPIOB
#define CAN2_GPIO_PIN                           (GPIO_PIN_12 | GPIO_PIN_13)
#define CAN2_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()

#define CANx_GPIO_CLK_ENABLE(_id)               do { if (_id==xCAN1) CAN1_GPIO_CLK_ENABLE();\
                                                     else if (_id==xCAN2) CAN2_GPIO_CLK_ENABLE();\
                                                   } while(0)
#define xCAN_CLK_ENABLE()                       do { __HAL_RCC_CAN1_CLK_ENABLE();\
                                                     __HAL_RCC_CAN2_CLK_ENABLE();\
                                                   } while(0)

#define CHASSIS_CAN_ID                          2
#define CHASSIS_CAN_INSTANCE                    EVALUATOR2(CAN, CHASSIS_CAN_ID)
#define CHASSIS_CAN_TX                          EVALUATOR3(Can, CHASSIS_CAN_ID, _TxMsg)
#define CHASSIS_CAN_RX                          EVALUATOR3(Can, CHASSIS_CAN_ID, _RxMsg)
#define CHASSIS_CAN_HANDLE                      EVALUATOR3(Can, CHASSIS_CAN_ID, _Handle)

#endif // BOARD_INFO_H
