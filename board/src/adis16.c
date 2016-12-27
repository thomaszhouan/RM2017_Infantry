#define ADIS16_FILE

#include "common.h"
#include "board_info.h"
#include "adis16.h"
#include "dwt.h"
#include <string.h>

/* ADIS16 register address */
#define ADIS16_FLASH              0x01
#define ADIS16_POWER              0x03
#define ADIS16_VEL                0x05
#define ADIS16_ADC                0x0B
#define ADIS16_TEMP               0x0D
#define ADIS16_ANGL               0x0F
#define ADIS16_OFF                0x15   
#define ADIS16_COMD               0x3F
#define ADIS16_SENS               0x39
#define ADIS16_SMPL               0x37

#define MIN_OMEGA                 10
#define TEMPERATURE_SCALE         0.1453f
#define TEMPERATURE_OFFSET        25.0f
#define BIAS_TEMP_COEFFICIENT     0.0051f

#define CHIP_SELECT()             HAL_GPIO_WritePin(ADIS16_CS_PORT, ADIS16_CS_PIN, GPIO_PIN_RESET)
#define CHIP_DESELECT()           HAL_GPIO_WritePin(ADIS16_CS_PORT, ADIS16_CS_PIN, GPIO_PIN_SET)

static void ADIS16_SPI_Init(void);
static void ADIS16_Write(uint8_t addr, uint16_t data);
static uint16_t ADIS16_Read(uint8_t addr);
static uint16_t ADIS16_SPI_IO(uint16_t data);

static int16_t ADIS16_GetOmega(void);
static uint16_t ADIS16_GetTheta(void);
static int16_t ADIS16_GetTemperature(void);

static float omegaIntegral;
static float initialTemperature;

#define omegaBufferSize           1
static int32_t omegaBuffer[omegaBufferSize];
static uint8_t omegaBufferId;

void ADIS16_Init(void) {
    /* Data initialization */
    ADIS16_Data.omega = 0;
    ADIS16_Data.theta = 0;
    ADIS16_Data.temperature = 25.0f;
    ADIS16_Data.lastUpdateTick = 0;
    omegaIntegral = 0.0f;
    omegaBufferId = 0;
    memset(omegaBuffer, 0, sizeof(omegaBuffer));
    ADIS16_DataUpdated = 0;

    /* Peripheral initialization */
    ADIS16_SPI_Init();

    /* Reset */
    ADIS16_Write(ADIS16_COMD, 0x0080);
    HAL_Delay(50);

    /* Factory Calibration Restore */
    ADIS16_Write(ADIS16_COMD, 0x0002);

    /* Filter settings */
    ADIS16_Write(ADIS16_SENS, 0x0404);
    ADIS16_Write(ADIS16_SMPL, 0x0001); // sample period: 3.906ms
    ADIS16_Write(ADIS16_COMD, 0x0008);
    HAL_Delay(100);
}

void ADIS16_Update(void) {
    uint32_t tick = HAL_GetTick();
    if (tick - ADIS16_Data.lastUpdateTick < 4)
        return;
    ADIS16_DataUpdated = 1;

    ADIS16_Data.lastUpdateTick = tick;
    ADIS16_Data.omegaHW = ADIS16_GetOmega();
    ADIS16_Data.thetaHW = ADIS16_GetTheta();
    ADIS16_Data.temperatureHW = ADIS16_GetTemperature();
    ADIS16_Data.temperature = TEMPERATURE_OFFSET +
        ADIS16_Data.temperatureHW * TEMPERATURE_SCALE;

    /* Omega filter */
    ADIS16_Data.omegaInternal -= omegaBuffer[omegaBufferId];
    ADIS16_Data.omegaInternal += (omegaBuffer[omegaBufferId] = ADIS16_Data.omegaHW);
    omegaBufferId = (omegaBufferId+1)&(omegaBufferSize-1);
    ADIS16_Data.omega = -(ADIS16_Data.omegaInternal / omegaBufferSize);

    /* Integration */
    float delta = ADIS16_Data.omegaHW -
        BIAS_TEMP_COEFFICIENT * (ADIS16_Data.temperature - initialTemperature);
    if (ABS(delta) < MIN_OMEGA)
        delta = 0;
    omegaIntegral += delta;
    if (omegaIntegral < 0) omegaIntegral += 1258065;
    else if (omegaIntegral >= 1258065) omegaIntegral -= 1258065;
    ADIS16_Data.theta = omegaIntegral * 0.07326f * 0.03906f;
}

void ADIS16_Calibrate(uint16_t sample) {
    HAL_Delay(2000);

    initialTemperature = TEMPERATURE_OFFSET +
        ADIS16_GetTemperature()*TEMPERATURE_SCALE;
    uint16_t originalOffset = ADIS16_Read(ADIS16_OFF);
    int32_t offsetSum = 0;
    for (uint16_t i = 0; i < sample; ++i) {
        offsetSum += ADIS16_GetOmega();
        HAL_Delay(4);
    }

    int16_t offsetAverage = (int16_t)originalOffset-(int16_t)(offsetSum*4.0f/sample);
    uint16_t data = 0x0000U | offsetAverage;
    ADIS16_Write(ADIS16_OFF, data);
    ADIS16_Write(ADIS16_COMD, 0x0008);

    HAL_Delay(100);
}

/* 14 bit signed */
static int16_t ADIS16_GetOmega(void) {
    uint16_t buf = ADIS16_Read(ADIS16_VEL);

    // 14 bit to 16 bit sign extension
    if (buf & 0x2000) buf |= 0xC000;
    else buf &= 0x3FFF;

    int16_t ret = 0x0000 | buf;
    return ret;
}

/* 14 bit unsigned */
static uint16_t ADIS16_GetTheta(void) {
    uint16_t buf = ADIS16_Read(ADIS16_ANGL);
    return buf & 0x3FFFU;
}

/* 12 bit signed */
static int16_t ADIS16_GetTemperature(void) {
    uint16_t buf = ADIS16_Read(ADIS16_TEMP);
    if (buf & 0x0800) buf |= 0xF000;
    else buf &= 0x0FFF;
    int16_t ret = 0x0000 | buf;
    return ret;
}

static void ADIS16_SPI_Init(void) {
    // GPIO Init
    ADIS16_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pin = ADIS16_CS_PIN;
    HAL_GPIO_Init(ADIS16_CS_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(ADIS16_CS_PORT, ADIS16_CS_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Pin = ADIS16_SPI_PIN;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(ADIS16_SPI_PORT, &GPIO_InitStruct);

    // SPI Init
    ADIS16_SPI_CLK_ENABLE();
    ADIS16_SPI_HANDLE.Instance = ADIS16_SPI_INSTANCE;
    ADIS16_SPI_HANDLE.Init.Mode = SPI_MODE_MASTER;
    ADIS16_SPI_HANDLE.Init.Direction = SPI_DIRECTION_2LINES;
    ADIS16_SPI_HANDLE.Init.DataSize = SPI_DATASIZE_16BIT;
    ADIS16_SPI_HANDLE.Init.CLKPolarity = SPI_POLARITY_HIGH;
    ADIS16_SPI_HANDLE.Init.CLKPhase = SPI_PHASE_2EDGE;
    ADIS16_SPI_HANDLE.Init.NSS = SPI_NSS_SOFT;
    ADIS16_SPI_HANDLE.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    ADIS16_SPI_HANDLE.Init.FirstBit = SPI_FIRSTBIT_MSB;
    ADIS16_SPI_HANDLE.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    HAL_SPI_DeInit(&ADIS16_SPI_HANDLE);
    HAL_SPI_Init(&ADIS16_SPI_HANDLE);
    __HAL_SPI_ENABLE(&ADIS16_SPI_HANDLE);
}

/*
       addr-1          addr  
    data[7:0]    data[15:8]
*/
static void ADIS16_Write(uint8_t addr, uint16_t data) {
    addr = (addr & 0x3F) | 0x80;

    uint16_t cmd;

    cmd = ((uint16_t)addr << 8) | (data >> 8);
    ADIS16_SPI_IO(cmd);

    cmd = ((uint16_t)(addr-1) << 8) | (data & 0xFF);
    ADIS16_SPI_IO(cmd);
}

static uint16_t ADIS16_Read(uint8_t addr) {
    addr &= 0x3F;
    uint16_t cmd = addr << 8;

    ADIS16_SPI_IO(cmd);
    return ADIS16_SPI_IO(cmd);
}

/*
    Full duplex.
*/
static uint16_t ADIS16_SPI_IO(uint16_t data) {
    CHIP_SELECT();
    while (!__HAL_SPI_GET_FLAG(&ADIS16_SPI_HANDLE, SPI_FLAG_TXE))
        ;
    ADIS16_SPI_HANDLE.Instance->DR = data;

    while (!__HAL_SPI_GET_FLAG(&ADIS16_SPI_HANDLE, SPI_FLAG_RXNE))
        ;
    uint16_t ret = ADIS16_SPI_HANDLE.Instance->DR;
    CHIP_DESELECT();

    /* Minimum stall time = 9us */
    DWT_DelayUs(10);
    return ret;
}