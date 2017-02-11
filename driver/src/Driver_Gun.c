#define GUN_FILE

#include "Driver_Dbus.h"
#include "Driver_Encoder.h"
#include "Driver_Gun.h"
#include "PID.h"

#include <string.h>

static PID_Controller PokeController;

void GUN_Init(void) {
    memset((char*)&GUN_Data, 0, sizeof(GUN_Data));

    PID_Reset(&PokeController);
    PokeController.Kp = 150.0f;
    PokeController.Ki = 24.00f;
    PokeController.Kd = 0.00f;
    PokeController.MAX_Pout = 12000;
    PokeController.MAX_Integral = 100000;
    PokeController.MAX_PIDout = 12000;
    PokeController.MIN_PIDout = 0;
    PokeController.mode = kPositional;
}

void GUN_SetMotion(void) {
    if (DBUS_Data.rightSwitchState != kSwitchMiddle)
        GUN_Data.pokeTargetSpeed = 0;
    else
        GUN_Data.pokeTargetSpeed = 160;
    // GUN_Data.pokeTargetSpeed = DBUS_Data.ch2 / 4;
}

void GUN_PokeControl(void) {
    ENCODER_Update();
    GUN_Data.pokeOutput = PID_Update(&PokeController,
        GUN_Data.pokeTargetSpeed, ENCODER_Data);
    if (DBUS_Status == kLost)
        GUN_SetFree();

#if POKE_DIR == 0
    if (GUN_Data.pokeOutput >= 0) {
        GPIO_SetBits(POKE_DIR_PORT, POKE_DIR_PIN);
        POKE_SET_PWM(GUN_Data.pokeOutput);
    }
    else {
        GPIO_ResetBits(POKE_DIR_PORT, POKE_DIR_PIN);
        POKE_SET_PWM(-GUN_Data.pokeOutput);
    }
#else
    if (GUN_Data.pokeOutput >= 0) {
        GPIO_ResetBits(POKE_DIR_PORT, POKE_DIR_PIN);
        POKE_SET_PWM(GUN_Data.pokeOutput);
    }
    else {
        GPIO_SetBits(POKE_DIR_PORT, POKE_DIR_PIN);
        POKE_SET_PWM(-GUN_Data.pokeOutput);
    }
#endif
}

void GUN_SetFree(void) {
    PID_Reset(&PokeController);
    memset((char*)&GUN_Data, 0, sizeof(GUN_Data));
}
