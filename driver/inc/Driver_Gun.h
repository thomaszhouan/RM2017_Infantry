#ifndef DRIVER_GUN
#define DRIVER_GUN

#include "stm32f4xx.h"

#ifndef GUN_FILE
    #define GUN_EXT extern
#else
    #define GUN_EXT
#endif

#define POKE_DIR_PORT                           GPIOA
#define POKE_DIR_PIN                            GPIO_Pin_1
#define POKE_DIR                                1
#define POKE_SET_PWM(x)                         TIM_SetCompare1(TIM2, x)

typedef struct {
    int32_t pokeTargetSpeed;
    int32_t pokeOutput;
} GUN_DataTypeDef;

GUN_EXT volatile GUN_DataTypeDef GUN_Data;

void GUN_Init(void);
void GUN_SetMotion(void);
void GUN_PokeControl(void);
void GUN_SetFree(void);

void GUN_ShootOne(void);

#endif
