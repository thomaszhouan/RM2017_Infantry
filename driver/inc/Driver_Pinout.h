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

#endif
