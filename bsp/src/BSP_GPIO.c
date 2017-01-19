#include "stm32f4xx.h"
#include "BSP_GPIO.h"
#include "Driver_Pinout.h"

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

    // LED
    GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin    =   LED_PIN;
    GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_2MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    GPIO_SetBits(LED_PORT, LED_PIN);

    // // CAN1 (PA11 PA12)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_11 | GPIO_Pin_12;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    // GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    // CAN2 (PB12 PB13)
    GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin    =   CAN2_PIN;
    GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    GPIO_Init(CAN2_PORT, &GPIO_InitStructure);
    CAN2_AF_CONFIG();

    // USART1 (PB6 PB7)
    GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin    =   USART1_PIN;
    GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    GPIO_Init(USART1_PORT, &GPIO_InitStructure);
    USART1_AF_CONFIG();

    // // USART3 (PB10 PB11)
    // GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    // GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    // GPIO_InitStructure.GPIO_Pin    =   GPIO_Pin_10 | GPIO_Pin_11;
    // GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    // GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    // GPIO_Init(GPIOB, &GPIO_InitStructure);
    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    // GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

    // ST7735
    GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin    =   ST7735_SPI_PIN;
    GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    GPIO_Init(ST7735_SPI_PORT, &GPIO_InitStructure);
    ST7735_SPI_AF_CONFIG();

    GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_2MHz;

    GPIO_InitStructure.GPIO_Pin    =   ST7735_RST_PIN;
    GPIO_Init(ST7735_RST_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin    =   ST7735_CS_PIN;
    GPIO_Init(ST7735_CS_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin    =   ST7735_DC_PIN;
    GPIO_Init(ST7735_DC_PORT, &GPIO_InitStructure);

    // ADIS16
    GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =   GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Pin    =   ADIS16_SPI_PIN;
    GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_100MHz;
    GPIO_Init(ADIS16_SPI_PORT, &GPIO_InitStructure);
    ADIS16_AF_CONFIG();

    GPIO_InitStructure.GPIO_Mode   =   GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  =   GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Pin    =   ADIS16_CS_PIN;
    GPIO_InitStructure.GPIO_PuPd   =   GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =   GPIO_Speed_2MHz;
    GPIO_Init(ADIS16_CS_PORT, &GPIO_InitStructure);
    GPIO_SetBits(ADIS16_CS_PORT, ADIS16_CS_PIN);
}
