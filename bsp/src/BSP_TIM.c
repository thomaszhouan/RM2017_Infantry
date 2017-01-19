#include "BSP_TIM.h"
#include "stm32f4xx.h"

/**
  * @brief  TIM Init
  * @param  void
  * @retval void
  */
void BSP_TIM_InitConfig(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // TIM2 (main control 1kHz)
    TIM_TimeBaseInitStructure.TIM_ClockDivision =   TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode   =   TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period        =   1000;
    TIM_TimeBaseInitStructure.TIM_Prescaler     =   84-1; // 1MHz clock
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
        
    // // TIM3 (gyro update 256Hz)
    // TIM_TimeBaseInitStructure.TIM_ClockDivision =   TIM_CKD_DIV1;
    // TIM_TimeBaseInitStructure.TIM_CounterMode   =   TIM_CounterMode_Up;
    // TIM_TimeBaseInitStructure.TIM_Period        =   125;
    // TIM_TimeBaseInitStructure.TIM_Prescaler     =   (uint32_t) (((SystemCoreClock / 2) / 32000)-1);
    // TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
}

void BSP_TIM_Start(void) {
    // TIM2
    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    // TIM3
    TIM_Cmd(TIM3, ENABLE);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
}
