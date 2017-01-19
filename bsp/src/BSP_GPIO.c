#include "BSP_GPIO.h"
#include "stm32f4xx.h"

/**
  * @brief  GPIO Init
  * @param  void
  * @retval void 
  */
void BSP_GPIO_InitConfig(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // LED (PB3)
    GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_3);

    // // CAN1 (PA11 PA12)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_11 | GPIO_Pin_12;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    // // CAN2 (PB12 PB13)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_12 | GPIO_Pin_13;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

    // // USART1 (PB6 PB7)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_6 | GPIO_Pin_7;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_100MHz;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

    // // USART3 (PB10 PB11)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_10 | GPIO_Pin_11;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    // // SPI1 (PA5 PA7)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_5 | GPIO_Pin_7;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    // // SPI3 (PC10 PC11 PC12)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    // GPIO_Init(GPIOC, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    // GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
    // GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

    // // LCD GPIO (PB0 PC4 PC5)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_OUT;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_4 | GPIO_Pin_5;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_Init(GPIOC, &GPIO_InitStructure);

    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_0;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
}
