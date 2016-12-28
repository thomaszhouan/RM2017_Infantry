#define CHASSIS_FILE

#include "common.h"
#include "board_info.h"
#include "chassis.h"
#include "dbus.h"
#include "can.h"
#include "adis16.h"
#include "judge.h"
#include <string.h>

#define _ID(x) ((x)-CHASSIS_CAN_ID_OFFSET)

/* 1: normal 0: reverse */
static const char MOTOR_DIR[4] = {1, 0, 0, 1};
static PID_Controller MotorController[4];
static volatile int16_t MotorVelocityBuffer[4][2];
static volatile uint8_t BufferId[4];

static PID_Controller ChassisAngleController;
static volatile int32_t ChassisOmegaOutput;
static volatile int32_t targetOmega = 0;

static PID_Controller ChassisPowerController;

#define _CLEAR(x) do { memset((void*)(x), 0, sizeof(x)); } while(0)

static int32_t CHASSIS_Trim(int32_t val, int32_t lim) {
    if (val > lim) val = lim;
    if (val < -lim) val = -lim;
    return val;
}

static int32_t CHASSIS_Clamp(int32_t val, int32_t min, int32_t max) {
    if (val < min) return min;
    else if (val > max) return max;
    else return val;
}

static void CHASSIS_ClearAll(void) {
    _CLEAR(MotorAngle);
    _CLEAR(MotorLastAngle);
    _CLEAR(MotorVelocity);
    _CLEAR(MotorVelocityBuffer);
    _CLEAR(BufferId);
    _CLEAR(TargetVelocity);
    _CLEAR(MotorOutput);
    _CLEAR(MeasureUpdated);
    ChassisOmegaOutput = 0;
}

static void CHASSIS_CanInit(void) {
    // CAN
    CAN_SimpleInitTypeDef CAN_InitStruct;
    CAN_InitStruct.id                 = CHASSIS_CAN_ID;
    CAN_InitStruct.CanHandle          = &CHASSIS_CAN_HANDLE;
    CAN_InitStruct.CanRx              = &CHASSIS_CAN_RX;
    CAN_InitStruct.CanTx              = &CHASSIS_CAN_TX;
    CAN_InitStruct.PreemptionPriority = 9;
    CAN_InitStruct.SubPriority        = 0;
    CAN_Init(&CAN_InitStruct);

    // CAN Tx
    CHASSIS_CAN_TX.StdId = CHASSIS_MASTER_ID;
    CHASSIS_CAN_TX.IDE   = CAN_ID_STD;
    CHASSIS_CAN_TX.RTR   = CAN_RTR_DATA;
    CHASSIS_CAN_TX.DLC   = 8;
}

void CHASSIS_Init(void) {
    CHASSIS_CanInit();

    /* Motor controller */
    for (uint8_t id = 0; id < 4; ++id) {
        PID_Reset(MotorController+id);
        MotorController[id].Kp = CHASSIS_KP;
        MotorController[id].Ki = CHASSIS_KI;
        MotorController[id].Kd = CHASSIS_KD;
        MotorController[id].MAX_Pout = CHASSIS_MAX_POUT;
        MotorController[id].MAX_Integral = CHASSIS_MAX_INTEGRAL;
        MotorController[id].MAX_PIDout = CHASSIS_MAX_PIDOUT;
        MotorController[id].MIN_PIDout = CHASSIS_MIN_PIDOUT;
        MotorController[id].mode = CHASSIS_PID_MODE;
    }

    /* Chassis angle controller */
    PID_Reset(&ChassisAngleController);
    ChassisAngleController.Kp = CHASSIS_OMEGA_KP;
    ChassisAngleController.Ki = CHASSIS_OMEGA_KI;
    ChassisAngleController.Kd = CHASSIS_OMEGA_KD;
    ChassisAngleController.MAX_Pout = CHASSIS_OMEGA_MAX_POUT;
    ChassisAngleController.MAX_Integral = CHASSIS_OMEGA_MAX_INTEGRAL;
    ChassisAngleController.MAX_PIDout = CHASSIS_OMEGA_MAX_PIDOUT;
    ChassisAngleController.MIN_PIDout = CHASSIS_OMEGA_MIN_PIDOUT;
    ChassisAngleController.mode = CHASSIS_OMEGA_PID_MODE;

    /* Chassis power controller */
    PID_Reset(&ChassisPowerController);
    ChassisPowerController.Kp = 0.005f;
    ChassisPowerController.Ki = 0.008f;
    ChassisPowerController.Kd = 0.000f;
    ChassisPowerController.IDecayFactor = 0.9f;
    ChassisPowerController.MAX_Pout = 100;
    ChassisPowerController.MAX_Integral = 100;
    ChassisPowerController.MAX_PIDout = 1;
    ChassisPowerController.MIN_PIDout = 0;
    ChassisPowerController.mode = kIntegralDecay;
    ChassisPowerRatio = 1.0f;

    CHASSIS_ClearAll();
    _CLEAR(MotorFeedbackCount);

    HAL_CAN_Receive_IT(&CHASSIS_CAN_HANDLE, 0);
}

void CHASSIS_UpdateMeasure(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    uint8_t *data = CHASSIS_CAN_RX.Data;
    MotorLastAngle[id] = MotorAngle[id];
    MotorAngle[id] = ((uint16_t)data[0]<<8) | ((uint16_t)data[1]);
    int16_t newVelocity = ((uint16_t)data[2]<<8) | ((uint16_t)data[3]);
    ++MotorFeedbackCount[id];

    /* overflow protection */
    if (TargetVelocity[id] > 10 && newVelocity < -3000) newVelocity += 16384;
    else if (TargetVelocity[id] < -10 && newVelocity > 3000) newVelocity -= 16384;

    MotorVelocity[id] -= MotorVelocityBuffer[id][BufferId[id]];
    MotorVelocityBuffer[id][BufferId[id]] = newVelocity;
    MotorVelocity[id] += newVelocity;
    BufferId[id] = (BufferId[id]+1)&0x1U;

    MeasureUpdated[id] = 1;
}

void CHASSIS_MotorControl(uint16_t motorId) {
    uint16_t id = _ID(motorId);
    if (!MeasureUpdated[id]) return;
    MotorOutput[id] = (int16_t)PID_Update(MotorController+id,
        ChassisPowerRatio*TargetVelocity[id], MotorVelocity[id]/2);
    
    MeasureUpdated[id] = 0;
}

void CHASSIS_Control(void) {
    if (JUDGE_Data.powerUpdated) {
        JUDGE_Data.powerUpdated = 0;
        CHASSIS_PowerControl();
    }
    CHASSIS_MotorControl(FL_MOTOR_ID);
    CHASSIS_MotorControl(FR_MOTOR_ID);
    CHASSIS_MotorControl(BR_MOTOR_ID);
    CHASSIS_MotorControl(BL_MOTOR_ID);
}

/*
    y
    ^
    |
    0-- >x
    Rotation: CW as +ve
*/
void CHASSIS_SetMotion(void) {
    static int32_t velocityX = 0, velocityY = 0;
    static int32_t tmpVelocity[4];
    static const int32_t maxDelta = 250;

    velocityX = 18 * DBUS_Data.ch1;
    velocityY = 12 * DBUS_Data.ch2;
    targetOmega = 20 * DBUS_Data.ch3;

    /*
        Chassis angle control mode (DBUS right switch):
        kSwitchDown: open loop
        kSwitchMiddle: omega close loop
        kSwitchUp: angle close loop (not supported now)
    */
    if (DBUS_Data.rightSwitchState == kSwitchDown) {
        if (DBUS_LastData.rightSwitchState == kSwitchMiddle)
            PID_Reset(&ChassisAngleController);
        ChassisOmegaOutput = (ChassisOmegaOutput*7+8*DBUS_Data.ch3)/8;
    }
    else if (DBUS_Data.rightSwitchState == kSwitchMiddle) {
        if (ADIS16_DataUpdated) {
            ADIS16_DataUpdated = 0;
            CHASSIS_RotationControl();
        }
    }
    else { // DBUS_Data.rightSwitchState == kSwitchUp
        /* temporary, same as kSwitchDown */
        if (DBUS_LastData.rightSwitchState == kSwitchMiddle)
            PID_Reset(&ChassisAngleController);
        ChassisOmegaOutput = (ChassisOmegaOutput*7+8*DBUS_Data.ch3)/8;
    }

    tmpVelocity[0] = velocityY + velocityX + ChassisOmegaOutput;
    tmpVelocity[1] = velocityY - velocityX - ChassisOmegaOutput;
    tmpVelocity[2] = velocityY + velocityX - ChassisOmegaOutput;
    tmpVelocity[3] = velocityY - velocityX + ChassisOmegaOutput;

    for (uint8_t i = 0; i < 4; ++i) {
        int32_t tmp = MOTOR_DIR[i] ? TargetVelocity[i] : -TargetVelocity[i];
        tmpVelocity[i] = CHASSIS_Clamp(tmpVelocity[i],
            tmp-maxDelta, tmp+maxDelta);
        CHASSIS_SetTargetVelocity(i+CHASSIS_CAN_ID_OFFSET, tmpVelocity[i]);
    }
}

void CHASSIS_RotationControl(void) {
    ChassisOmegaOutput = (int32_t)PID_Update(&ChassisAngleController,
        targetOmega, ADIS16_Data.omega);
}

void CHASSIS_PowerControl(void) {
    static float reducedRatio = 0.0f;
    reducedRatio = PID_Update(&ChassisPowerController,
        CHASSIS_ENERGY, JUDGE_Data.remainEnergy);
    if (reducedRatio < 0.0f)
        reducedRatio = 0.0f;
    ChassisPowerRatio = 1.0f - reducedRatio;
    // ChassisPowerRatio = 1.0f;
}

void CHASSIS_SetTargetVelocity(uint16_t motorId, int32_t velocity) {
    uint16_t id = _ID(motorId);
    velocity = CHASSIS_Trim(velocity, MAX_TARGET_VELOCITY);
    TargetVelocity[id] = (int16_t)velocity;
    if (!MOTOR_DIR[id]) TargetVelocity[id] = -TargetVelocity[id];
}

void CHASSIS_SendCmd(void) {
    static uint8_t *data = CHASSIS_CAN_TX.Data;
    data[0] = (MotorOutput[0]&0xFF00)>>8;
    data[1] = MotorOutput[0]&0x00FF;
    data[2] = (MotorOutput[1]&0xFF00)>>8;
    data[3] = MotorOutput[1]&0x00FF;
    data[4] = (MotorOutput[2]&0xFF00)>>8;
    data[5] = MotorOutput[2]&0x00FF;
    data[6] = (MotorOutput[3]&0xFF00)>>8;
    data[7] = MotorOutput[3]&0x00FF;
    HAL_CAN_Transmit_IT(&CHASSIS_CAN_HANDLE);
}

void CHASSIS_SetFree(void) {
    for (uint8_t i = 0; i < 4; ++i)
        PID_Reset(MotorController+i);
    PID_Reset(&ChassisAngleController);
    CHASSIS_ClearAll();
}
