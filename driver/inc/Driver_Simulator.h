#ifndef DRIVER_SIMULATOR
#define DRIVER_SIMULATOR

void SIMULATOR_Init(void);
void SIMULATOR_SendHeartBeat(void);
void SIMULATOR_CameraInit(uint8_t robotId);
void SIMULATOR_ArmorInit(uint8_t robotId);

#endif
