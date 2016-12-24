#ifndef JUDGE_H
#define JUDGE_H

#define JUDGE_UART              EVALUATOR2(UART, JUDGE_UART_NUM)
#define JUDGE_UART_HANDLE       EVALUATOR3(Uart, JUDGE_UART_NUM, _Handle)
#define JUDGE_DMA_HANDLE        EVALUATOR3(Uart, JUDGE_UART_NUM, _RxDmaHandle)

#define JudgeBufferLength       150
#define JudgeFrameLength_1      46
#define JudgeFrameLength_2      11
#define JudgeFrameLength_3      24

#define JudgeFrameHeader        0xA5

#ifndef JUDGE_FILE
    #define JUDGE_EXT extern
#else
    #define JUDGE_EXT
#endif

// BUFF
typedef enum {
    BUFF_TYPE_NONE,
    BUFF_TYPE_ARMOR = 0x01,
    BUFF_TYPE_SUPPLY = 0x04,
    BUFF_TYPE_BULLFTS= 0x08,
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

JUDGE_EXT uint8_t JUDGE_DataBuffer[JudgeBufferLength];
// JUDGE_EXT InfantryJudge_Struct InfantryJudge;
JUDGE_EXT float JUDGE_FrameCounter;
JUDGE_EXT float JUDGE_FrameRate;

void JUDGE_Init(void);

/* provided by DJI */
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);

#endif // JUDGE_H
