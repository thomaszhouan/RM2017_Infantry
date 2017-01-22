#ifndef DRIVER_PINOUT_H
#define DRIVER_PINOUT_H

// LED
#define LED_PORT                                GPIOB
#define LED_PIN                                 GPIO_Pin_3

// ST7735
#define ST7735_SPI                              SPI1
#define ST7735_SPI_PORT                         GPIOA
#define ST7735_SPI_PIN                          (GPIO_Pin_5 | GPIO_Pin_7)
#define ST7735_SPI_AF_CONFIG()                  do { GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);\
                                                     GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);\
                                                } while(0)
#define ST7735_RST_PORT                         GPIOB
#define ST7735_RST_PIN                          GPIO_Pin_0
#define ST7735_CS_PORT                          GPIOC
#define ST7735_CS_PIN                           GPIO_Pin_4
#define ST7735_DC_PORT                          GPIOC
#define ST7735_DC_PIN                           GPIO_Pin_5

// USART1 (DBUS)
#define USART1_PORT                             GPIOB
#define USART1_PIN                              (GPIO_Pin_6 | GPIO_Pin_7)
#define USART1_AF_CONFIG()                      do { GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);\
                                                     GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);\
                                                } while(0)

// USART3 (Judge)
#define USART3_PORT                             GPIOB
#define USART3_PIN                              (GPIO_Pin_10 | GPIO_Pin_11)
#define USART3_AF_CONFIG()                      do { GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);\
                                                     GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);\
                                                } while(0)

// CAN1 (Chassis)
#define CAN1_PORT                               GPIOA
#define CAN1_PIN                                (GPIO_Pin_11 | GPIO_Pin_12)
#define CAN1_AF_CONFIG()                        do { GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);\
                                                     GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);\
                                                } while(0)

// CAN2 (Chassis)
#define CAN2_PORT                               GPIOB
#define CAN2_PIN                                (GPIO_Pin_12 | GPIO_Pin_13)
#define CAN2_AF_CONFIG()                        do { GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);\
                                                     GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);\
                                                } while(0)

// ADIS16
#define ADIS16_SPI                              SPI3
#define ADIS16_CS_PORT                          GPIOD
#define ADIS16_CS_PIN                           GPIO_Pin_2
#define ADIS16_SPI_PORT                         GPIOC
#define ADIS16_SPI_PIN                          (GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12)
#define ADIS16_AF_CONFIG()                      do { GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);\
                                                     GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);\
                                                     GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);\
                                                } while(0)

#endif
