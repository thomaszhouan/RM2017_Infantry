#define CHASSIS_FILE

#include "Driver_ADIS16.h"
#include "Driver_Chassis.h"
#include "Driver_Dbus.h"
#include "Driver_Judge.h"
#include "Param.h"
#include <string.h>

#define _ID(x) ((x)-CHASSIS_CAN_ID_OFFSET)

/* 1: normal 0: reverse */
static const char MOTOR_DIR[4] = {1, 0, 0, 1};
static PID_Controller MotorController[4];
static volatile int16_t MotorVelocityBuffer[4][2];
static volatile uint8_t BufferId[4];

static CanTxMsg ChassisCanTx;

static PID_Controller ChassisAngleController;
static PID_Controller ChassisOmegaController;
// static volatile int32_t ChassisOmegaOutput;
// static volatile float targetAngle = 0.0f;
// static volatile int32_t targetOmega = 0;

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
    targetOmega = 0;
    angleError = 0;
}

void CHASSIS_Init(void) {
    ChassisCanTx.StdId = 0x200;
    ChassisCanTx.IDE = CAN_Id_Standard;
    ChassisCanTx.RTR = CAN_RTR_Data;
    ChassisCanTx.DLC = 8;

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

    /* Chassis omega controller */
    PID_Reset(&ChassisOmegaController);
    ChassisOmegaController.Kp = 0.10f;//CHASSIS_OMEGA_KP;
    ChassisOmegaController.Ki = 0.00f;//CHASSIS_OMEGA_KI;
    ChassisOmegaController.Kd = 0.00f;//CHASSIS_OMEGA_KD;
    ChassisOmegaController.MAX_Pout = CHASSIS_OMEGA_MAX_POUT;
    ChassisOmegaController.MAX_Integral = CHASSIS_OMEGA_MAX_INTEGRAL;
    ChassisOmegaController.MAX_PIDout = CHASSIS_OMEGA_MAX_PIDOUT;
    ChassisOmegaController.MIN_PIDout = CHASSIS_OMEGA_MIN_PIDOUT;
    ChassisOmegaController.mode = CHASSIS_OMEGA_PID_MODE;

    /* Chassis angle controller */
    PID_Reset(&ChassisAngleController);
    ChassisAngleController.Kp = 10.0f;//16.00f;
    ChassisAngleController.Ki = 0.0f;//0.40f;
    ChassisAngleController.Kd = 0.0f;//9.00f//0.10f;
    ChassisAngleController.MAX_Pout = 10000;
    ChassisAngleController.MAX_Integral = 10000;
    ChassisAngleController.MAX_PIDout = 15000;
    ChassisAngleController.MIN_PIDout = 0;
    ChassisAngleController.mode = kPositional;

    /* Chassis power controller */
    PID_Reset(&ChassisPowerController);
    ChassisPowerController.Kp = 0.005f;
    ChassisPowerController.Ki = 0.006f;
    ChassisPowerController.Kd = 0.000f;
    ChassisPowerController.IDecayFactor = 0.7f;
    ChassisPowerController.MAX_Pout = 100;
    ChassisPowerController.MAX_Integral = 100;
    ChassisPowerController.MAX_PIDout = 0.7;
    ChassisPowerController.MIN_PIDout = 0;
    ChassisPowerController.mode = kIntegralDecay;
    ChassisPowerRatio = 1.0f;

    CHASSIS_ClearAll();
    _CLEAR(MotorFeedbackCount);
    targetAngle = 0.0f;
}

void CHASSIS_UpdateMeasure(uint16_t motorId, uint8_t *data) {
    uint16_t id = _ID(motorId);
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
    static const int32_t maxDelta = 300;

    int16_t vxData, vyData, rotData;

    /*
        Controller / KM control
        kSwitchDown: controller used
        kSwitchMiddle: keyboard + mouse used
    */
    if (DBUS_Data.leftSwitchState == kSwitchDown) {
        vxData = DBUS_Data.ch1;
        vyData = DBUS_Data.ch2;
        rotData = DBUS_Data.ch3;
    }
    else if (DBUS_Data.leftSwitchState == kSwitchMiddle) {
        if (DBUS_IsKeyPressed(KEY_D)) vxData = 660;
        else if (DBUS_IsKeyPressed(KEY_A)) vxData = -660;
        else vxData = 0;
        if (DBUS_IsKeyPressed(KEY_W)) vyData = 660;
        else if (DBUS_IsKeyPressed(KEY_S)) vyData = -660;
        else vyData = 0;
        if (DBUS_IsKeyPressed(KEY_E)) rotData = 660;
        else if (DBUS_IsKeyPressed(KEY_Q)) rotData = -660;
        else rotData = 0;
    }
    else { // DBUS_Data.leftSwitchState == kSwitchUp
        vxData = 0;
        vyData = 0;
        rotData = 0;
    }

    velocityX = 18 * vxData;
    velocityY = 12 * vyData;

    /*
        Chassis angle control mode (DBUS right switch):
        kSwitchDown: open loop
        kSwitchMiddle: omega close loop
        kSwitchUp: angle close loop
    */
    if (DBUS_Data.rightSwitchState == kSwitchDown) {
        ChassisOmegaOutput = (ChassisOmegaOutput*7+5*rotData)/8;
    }
    else if (DBUS_Data.rightSwitchState == kSwitchMiddle) {
        if (DBUS_LastData.rightSwitchState != kSwitchMiddle)
            PID_Reset(&ChassisOmegaController);
        if (ADIS16_DataUpdated) {
            ADIS16_DataUpdated = 0;
            targetOmega = 16 * rotData;
            
            ChassisOmegaOutput = (int32_t)PID_Update(&ChassisOmegaController,
                targetOmega, ADIS16_Data.omega);
        }
    }
    else { // DBUS_Data.rightSwitchState == kSwitchUp
        if (DBUS_LastData.rightSwitchState != kSwitchUp) {
            PID_Reset(&ChassisOmegaController);
            PID_Reset(&ChassisAngleController);
            targetAngle = ADIS16_Data.absoluteTheta;
        }
        targetAngle += 0.04f * rotData;
        angleError = (int32_t)(targetAngle - ADIS16_Data.absoluteTheta);
        angleError = CHASSIS_Trim(angleError, 300);
        targetAngle = angleError + ADIS16_Data.absoluteTheta;
        targetOmega = (int32_t)PID_Update(&ChassisAngleController,
            0.0f, -angleError);
        ChassisOmegaOutput = (int32_t)PID_Update(&ChassisOmegaController,
                targetOmega, ADIS16_Data.omega);
    }

    /* Mecanum wheel */
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
    ChassisOmegaOutput = (int32_t)PID_Update(&ChassisOmegaController,
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
    static uint8_t *data = ChassisCanTx.Data;

    data[0] = (MotorOutput[0]&0xFF00)>>8;
    data[1] = MotorOutput[0]&0x00FF;
    data[2] = (MotorOutput[1]&0xFF00)>>8;
    data[3] = MotorOutput[1]&0x00FF;
    data[4] = (MotorOutput[2]&0xFF00)>>8;
    data[5] = MotorOutput[2]&0x00FF;
    data[6] = (MotorOutput[3]&0xFF00)>>8;
    data[7] = MotorOutput[3]&0x00FF;
    CAN_Transmit(CAN2, &ChassisCanTx);
}

void CHASSIS_SetFree(void) {
    for (uint8_t i = 0; i < 4; ++i)
        PID_Reset(MotorController+i);
    PID_Reset(&ChassisOmegaController);
    PID_Reset(&ChassisAngleController);
    CHASSIS_ClearAll();
}
