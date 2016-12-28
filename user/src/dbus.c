#define DBUS_FILE

#include "common.h"
#include "board_info.h"
#include "dbus.h"
#include "uart.h"
#include <string.h>

#define _CLEAR(x) do { memset((void*)(&x), 0, sizeof(x)); } while(0)

void DBUS_Init(void) {
    DBUS_Status = kLost;
    DBUS_Data.ch1 = 0;
    DBUS_Data.ch2 = 0;
    DBUS_Data.ch3 = 0;
    DBUS_Data.ch4 = 0;
    DBUS_Data.leftSwitchState  = kSwitchDown;
    DBUS_Data.rightSwitchState = kSwitchDown;
    DBUS_LastData = DBUS_Data;
    DBUS_FrameCount = 0;
    DBUS_LastFrameCount = 0;

    UART_SimpleInitTypeDef UART_InitStruct;
    UART_InitStruct.Instance               = DBUS_UART;
    UART_InitStruct.UartHandle             = &DBUS_UART_HANDLE;
    UART_InitStruct.DmaHandleTx            = NULL;
    UART_InitStruct.DmaHandleRx            = &DBUS_DMA_HANDLE;
    UART_InitStruct.Baudrate               = 100000;
    UART_InitStruct.Parity                 = UART_PARITY_EVEN;
    UART_InitStruct.PreemptionPriority     = 15;
    UART_InitStruct.SubPriority            = 0;
    UART_InitStruct.DMA_Rx_Mode            = DMA_CIRCULAR;
    UART_InitStruct.DMA_PreemptionPriority = 12;
    UART_InitStruct.DMA_SubPriority        = 0;
    UART_Init(&UART_InitStruct);

    /* detect the first frame */
    __HAL_UART_ENABLE_IT(&DBUS_UART_HANDLE, UART_IT_IDLE);
}

void DBUS_Decode(void) {
    ++DBUS_FrameCount;

    DBUS_LastData = DBUS_Data;

    DBUS_Data.ch1 = (((uint32_t)DBUS_Buffer[0]) | ((uint32_t)DBUS_Buffer[1]<<8)) & 0x07FF;
    DBUS_Data.ch1 -= 1024;
    DBUS_Data.ch2 = (((uint32_t)DBUS_Buffer[1]>>3) | ((uint32_t)DBUS_Buffer[2]<<5)) & 0x07FF;
    DBUS_Data.ch2 -= 1024;
    DBUS_Data.ch3 = (((uint32_t)DBUS_Buffer[2]>>6) | ((uint32_t)DBUS_Buffer[3]<<2) | ((uint32_t)DBUS_Buffer[4]<<10)) & 0x07FF;
    DBUS_Data.ch3 -= 1024;
    DBUS_Data.ch4 = (((uint32_t)DBUS_Buffer[4]>>1) | ((uint32_t)DBUS_Buffer[5]<<7)) & 0x07FF;
    DBUS_Data.ch4 -= 1024;

    DBUS_Data.leftSwitchState = (DBUS_SwitchState)((DBUS_Buffer[5]>>6) & 0x03);
    DBUS_Data.rightSwitchState = (DBUS_SwitchState)((DBUS_Buffer[5]>>4) & 0x03);

    DBUS_Data.mouse.x = ((uint32_t)DBUS_Buffer[6]) | ((uint32_t)DBUS_Buffer[7]<<8);
    DBUS_Data.mouse.y = ((uint32_t)DBUS_Buffer[8]) | ((uint32_t)DBUS_Buffer[9]<<8);
    DBUS_Data.mouse.z = ((uint32_t)DBUS_Buffer[10]) | ((uint32_t)DBUS_Buffer[11]<<8);

    DBUS_Data.mouse.press_left = DBUS_Buffer[12];
    DBUS_Data.mouse.press_right = DBUS_Buffer[13];

    DBUS_Data.key.key_code = ((uint32_t)DBUS_Buffer[14]) | ((uint32_t)DBUS_Buffer[15]<<8);
}

void DBUS_UpdateStatus(void) {
    if (DBUS_FrameCount != DBUS_LastFrameCount) {
        DBUS_LastFrameCount = DBUS_FrameCount;
        DBUS_Status = kConnected;
    }
    else {
        DBUS_Status = kLost;
        _CLEAR(DBUS_Data);
        _CLEAR(DBUS_LastData);
        DBUS_Data.leftSwitchState  = kSwitchDown;
        DBUS_Data.rightSwitchState = kSwitchDown;
        DBUS_LastData.leftSwitchState  = kSwitchDown;
        DBUS_LastData.rightSwitchState = kSwitchDown;
    }
}
