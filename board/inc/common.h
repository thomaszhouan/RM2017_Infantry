/*
    common.h
    * header file from HAL
    * system clock config
*/

#ifndef COMMON_H
#define COMMON_H

#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"

#ifndef MIN
    #define MIN(a, b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
    #define MAX(a, b) (((a)>(b))?(a):(b))
#endif

#ifndef ABS
    #define ABS(x) (((x)>0)?(x):(-(x)))
#endif

#ifndef COUNTOF
    #define COUNTOF(_buffer) (sizeof(_buffer)/sizeof(*(_buffer)))
#endif

#define PASTER2(x, y) x ## y
#define EVALUATOR2(x, y) PASTER2(x, y)
#define PASTER3(x, y, z) x ## y ## z
#define EVALUATOR3(x, y, z) PASTER3(x, y, z)

typedef enum {
    kCoreClock,
    kSysTickExt
} Clock_t;

void SystemClock_Config(void);
uint32_t GetClock(Clock_t clockName);

#endif // COMMON_H
