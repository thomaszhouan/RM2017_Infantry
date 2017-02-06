#include "Driver_Menu.h"
#include "Driver_Pinout.h"
#include "Driver_ST7735.h"

void MENU_CheckJS(void) {
    if (GPIO_ReadInputDataBit(JS_PORT, GPIO_Pin_0) == RESET) {
        ST7735_Print(4, 6, GREEN, BLACK, "0");
    }
    else if (GPIO_ReadInputDataBit(JS_PORT, GPIO_Pin_1) == RESET) {
        ST7735_Print(4, 6, GREEN, BLACK, "1");
    }
    else if (GPIO_ReadInputDataBit(JS_PORT, GPIO_Pin_13) == RESET) {
        ST7735_Print(4, 6, GREEN, BLACK, "13");
    }
    else if (GPIO_ReadInputDataBit(JS_PORT, GPIO_Pin_14) == RESET) {
        ST7735_Print(4, 6, GREEN, BLACK, "14");
    }
    else if (GPIO_ReadInputDataBit(JS_PORT, GPIO_Pin_15) == RESET) {
        ST7735_Print(4, 6, GREEN, BLACK, "15");
    }
    else ST7735_Print(4, 6, GREEN, BLACK, "");
}
