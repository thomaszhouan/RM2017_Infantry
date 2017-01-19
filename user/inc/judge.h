#ifndef JUDGE_H
#define JUDGE_H

#include "stm32f4xx.h"

#define JUDGE_UART              EVALUATOR2(UART, JUDGE_UART_NUM)
#define JUDGE_UART_HANDLE       EVALUATOR3(Uart, JUDGE_UART_NUM, _Handle)
#define JUDGE_DMA_HANDLE        EVALUATOR3(Uart, JUDGE_UART_NUM, _RxDmaHandle)

#define JUDGE_BUFFER_LENGTH           150
#define JUDGE_INFO_FRAME_LENGTH       46
#define JUDGE_BLOOD_FRAME_LENGTH      11
#define JUDGE_SHOOT_FRAME_LENGTH      24

#define JUDGE_FRAME_HEADER        0xA5

#ifndef JUDGE_FILE
    #define JUDGE_EXT extern
#else
    #define JUDGE_EXT
#endif

// BUFF
typedef enum {
    BUFF_TYPE_NONE,
    BUFF_TYPE_ARMOR   = 0x01,
    BUFF_TYPE_SUPPLY  = 0x04,
    BUFF_TYPE_BULLFTS = 0x08,
} LBuffType_Enum;


// Postion data
typedef __packed struct {
    uint8_t flag; // 0: valid; 1: invalid
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t compass;
} GpsData_Struct;


// Game infomation
typedef __packed struct {
    uint32_t remainTime;
    uint16_t remainLifeValue;
    float realChassisOutV;
    float realChassisOutA;
    uint8_t runeStatus[4];
    uint8_t bigRune0Status;
    uint8_t bigRune1status;
    uint8_t conveyorBelts0:2;
    uint8_t conveyorBelts1:2;
    uint8_t parkingApron0:1;
    uint8_t parkingApron1:1;
    uint8_t parkingApron2:1;
    uint8_t parkingApron3:1;
    GpsData_Struct gpsData;
} GameInfo_Struct;


// Blood data
typedef __packed struct {
    uint8_t weakId:4;
    uint8_t way:4;
    uint16_t value;
} RealBloodChangedData_Struct;


// Shoot data
typedef __packed struct {
    float realBulletShootSpeed;
    float realBulletShootFreq;
    float realGolfShootSpeed;
    float realGolfShootFreq;
} RealShootData_Struct;

// union for format transform
typedef union {
    uint8_t U[4];
    float F;
} FormatTrans_TypeDef;

typedef struct {
    float voltage;              // V
    float current;              // A
    uint16_t remainLife;

    float power;                // W
    float remainEnergy;         // J
    uint8_t powerUpdated;

    int8_t hitArmorId;
    uint32_t lastHitTick;

    uint16_t shootNum;
    float shootSpeed;
    uint32_t lastShootTick;
} JUDGE_DecodeTypeDef;

JUDGE_EXT volatile uint8_t JUDGE_DataBuffer[JUDGE_BUFFER_LENGTH];
JUDGE_EXT volatile JUDGE_DecodeTypeDef JUDGE_Data;
JUDGE_EXT volatile uint32_t JUDGE_FrameCounter;

void JUDGE_Init(void);
void JUDGE_Decode(void);
void JUDGE_UpdatePower(void);

unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);

#endif // JUDGE_H
