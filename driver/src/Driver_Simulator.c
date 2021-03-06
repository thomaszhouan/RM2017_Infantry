#define SIMULATOR_FILE

#include "stm32f4xx.h"
#include "BSP_DWT.h"
#include "Driver_Simulator.h"
#include "Driver_Monitor.h"
#include "Driver_Judge.h"
#include "param.h"

static SimulatedData_Struct SIMULATOR_InfoData;

static const uint8_t NormalSequence[3][8] = {
    {0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
    {0xFF, 0x01, 0x00, 0x00, 0x15, 0xE5, 0x00, 0x65},
    {0xCB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

static const uint8_t InitSequence[58][3][8] = {
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x01, 0x6E},
     {0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x02, 0xF5},
     {0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x03, 0x7C},
     {0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x04, 0xC3},
     {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x05, 0x4A},
     {0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x06, 0xD1},
     {0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x07, 0x58},
     {0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x08, 0xAF},
     {0xCF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x09, 0x26},
     {0xDE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x0A, 0xBD},
     {0xEC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x0B, 0x34},
     {0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x0C, 0x8B},
     {0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x0D, 0x02},
     {0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x0E, 0x99},
     {0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x0F, 0x10},
     {0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x10, 0x66},
     {0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x11, 0xEF},
     {0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x12, 0x74},
     {0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x13, 0xFD},
     {0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x14, 0x42},
     {0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x15, 0xCB},
     {0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x16, 0x50},
     {0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x17, 0xD9},
     {0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x18, 0x2E},
     {0xDF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x19, 0xA7},
     {0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x1A, 0x3C},
     {0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x1B, 0xB5},
     {0xED, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x1C, 0x0A},
     {0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x1D, 0x83},
     {0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x1E, 0x18},
     {0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x1F, 0x91},
     {0xAB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x20, 0xE5},
     {0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x21, 0x6C},
     {0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x22, 0xF7},
     {0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x23, 0x7E},
     {0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x24, 0xC1},
     {0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x25, 0x48},
     {0x35, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x26, 0xD3},
     {0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x27, 0x5A},
     {0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x28, 0xAD},
     {0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x29, 0x24},
     {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x2A, 0xBF},
     {0xCD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x2B, 0x36},
     {0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x2C, 0x89},
     {0xA8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x2D, 0x00},
     {0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x2E, 0x9B},
     {0x8B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x2F, 0x12},
     {0x9A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x30, 0x64},
     {0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x31, 0xED},
     {0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x32, 0x76},
     {0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x33, 0xFF},
     {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x34, 0x40},
     {0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x35, 0xC9},
     {0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x36, 0x52},
     {0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x37, 0xDB},
     {0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x38, 0x2C},
     {0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF},
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x39, 0xA5},
     {0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    {{0x5A, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0xFF}, 
     {0xFF, 0x01, 0x00, 0x03, 0x15, 0xB0, 0x64, 0xC5}, 
     {0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
};

static const uint8_t CameraSequence[10][3][8] = {
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0xE9, 0x01, 0x01}, 
     {0x0D, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0x6B, 0x02, 0x02}, 
     {0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0x93, 0x01, 0x03}, 
     {0xF3, 0xA3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x04, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0x20, 0x01, 0x04}, 
     {0x9F, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x05, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0x1D, 0x01, 0x05}, 
     {0xE0, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x06, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0x5A, 0x01, 0x06}, 
     {0x61, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x07, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0xA2, 0x02, 0x07}, 
     {0xE2, 0x5D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x08, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0x25, 0x01, 0x08}, 
     {0xFB, 0x5C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x09, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0x18, 0x01, 0x09}, 
     {0x84, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}, 
    {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x06}, 
     {0x00, 0x01, 0x00, 0x04, 0x00, 0x5F, 0x01, 0x0A}, 
     {0x05, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
};

static const uint8_t ArmorSequence[4][10][3][8] = {
    // 0x140
    {{{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0x35, 0x01, 0x01},
      {0x44, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0xB7, 0x02, 0x02},
      {0x39, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0x4F, 0x01, 0x03},
      {0xBA, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0xFC, 0x01, 0x04},
      {0xD6, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x05, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0xC1, 0x01, 0x05},
      {0xA9, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x06, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0x86, 0x01, 0x06},
      {0x28, 0xEB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x07, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0x7E, 0x02, 0x07},
      {0xAB, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x08, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0xF9, 0x01, 0x08},
      {0xB2, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x09, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0xC4, 0x01, 0x09},
      {0xCD, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x02},
      {0x00, 0x01, 0x00, 0x04, 0x00, 0x83, 0x01, 0x0A},
      {0x4C, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},

    //0x141
    {{{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0xF8, 0x01, 0x01},
      {0x1E, 0x4A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0x7A, 0x02, 0x02},
      {0x63, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0x82, 0x01, 0x03},
      {0xE0, 0xC2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0x31, 0x01, 0x04},
      {0x8C, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x05, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0x0C, 0x01, 0x05},
      {0xF3, 0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x06, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0x4B, 0x01, 0x06},
      {0x72, 0x9F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x07, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0xB3, 0x02, 0x07},
      {0xF1, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x08, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0x34, 0x01, 0x08},
      {0xE8, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x09, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0x09, 0x01, 0x09},
      {0x97, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x02},
      {0x01, 0x01, 0x00, 0x04, 0x00, 0x4E, 0x01, 0x0A},
      {0x16, 0xB5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},

    // 0x142
    {{{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0xB6, 0x01, 0x01},
      {0xA3, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0x34, 0x02, 0x02},
      {0xDE, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0xCC, 0x01, 0x03},
      {0x5D, 0x5E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0x7F, 0x01, 0x04},
      {0x31, 0x8B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x05, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0x42, 0x01, 0x05},
      {0x4E, 0xCF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x06, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0x05, 0x01, 0x06},
      {0xCF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x07, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0xFD, 0x02, 0x07},
      {0x4C, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x08, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0x7A, 0x01, 0x08},
      {0x55, 0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x09, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0x47, 0x01, 0x09},
      {0x2A, 0xE5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x02},
      {0x02, 0x01, 0x00, 0x04, 0x00, 0x00, 0x01, 0x0A},
      {0xAB, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}},

    // 0x143
    {{{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0x7B, 0x01, 0x01},
      {0xF9, 0xA2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0xF9, 0x02, 0x02},
      {0x84, 0x89, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x03, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0x01, 0x01, 0x03},
      {0x07, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x04, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0xB2, 0x01, 0x04},
      {0x6B, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x05, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0x8F, 0x01, 0x05},
      {0x14, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x06, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0xC8, 0x01, 0x06},
      {0x95, 0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x02, 0x07, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0x30, 0x02, 0x07},
      {0x16, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x08, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0xB7, 0x01, 0x08},
      {0x0F, 0xD5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x09, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0x8A, 0x01, 0x09},
      {0x70, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     {{0x5A, 0x02, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x02},
      {0x03, 0x01, 0x00, 0x04, 0x00, 0xCD, 0x01, 0x0A},
      {0xF1, 0x5D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}}
};

static CanTxMsg CanTxData;
static uint8_t SampleCount;

static void VoltageConversion(void) {
    SIMULATOR_VoltageBuffer[SampleCount] =
        SIMULATOR_DataBuffer[3] * 0.008862305f;
}

static void CurrentConversion(void) {
    SIMULATOR_CurrentBuffer[SampleCount] = 
        (SIMULATOR_DataBuffer[11]-3172)*0.000805861/0.02f;
    if (SIMULATOR_CurrentBuffer[SampleCount] < 0.00001f)
        SIMULATOR_CurrentBuffer[SampleCount] = 0.00001f;
}

void SIMULATOR_Init(void) {
    CanTxData.StdId = 0x0FE;
    CanTxData.IDE = CAN_ID_STD;
    CanTxData.RTR = CAN_RTR_DATA;
    CanTxData.DLC = 8;
    for (uint16_t i = 0; i < 57; ++i) {
        for (int16_t j = 0; j < 3; ++j) {
            for (uint16_t k = 0; k < 8; ++k)
                CanTxData.Data[k] = InitSequence[i][j][k];
            CAN_Transmit(CAN1, &CanTxData);
        }
        SIMULATOR_SendHeartBeat();
    }

    SIMULATOR_Data.remainLife = 1500;
    SIMULATOR_Data.remainEnergy = 60.0f;
}

void SIMULATOR_SendHeartBeat(void) {
    CanTxData.StdId = 0x0FE;
    CanTxData.IDE = CAN_ID_STD;
    CanTxData.RTR = CAN_RTR_DATA;
    CanTxData.DLC = 8;
    for (uint16_t i = 0; i < 3; ++i) {
        for (uint16_t j = 0; j < 8; ++j)
            CanTxData.Data[j] = NormalSequence[i][j];
        CAN_Transmit(CAN1, &CanTxData);
        if (i==2) CanTxData.DLC = 2;
    }
}

void SIMULATOR_CameraInit(uint8_t robotId) {
    CanTxData.StdId = 0x130;
    CanTxData.IDE = CAN_ID_STD;
    CanTxData.RTR = CAN_RTR_DATA;
    CanTxData.DLC = 8;
    for (uint16_t i = 0; i < 3; ++i) {
        for (uint16_t j = 0; j < 8; ++j)
            CanTxData.Data[j] = CameraSequence[robotId][i][j];
        CAN_Transmit(CAN1, &CanTxData);
    }
}

void SIMULATOR_ArmorInit(uint8_t armorId, uint8_t robotId) {
    CanTxData.StdId = 0x140 | armorId;
    CanTxData.IDE = CAN_ID_STD;
    CanTxData.RTR = CAN_RTR_DATA;
    CanTxData.DLC = 8;
    for (uint16_t i = 0; i < 3; ++i) {
        for (uint16_t j = 0; j < 8; ++j)
            CanTxData.Data[j] = ArmorSequence[armorId][robotId][i][j];
        CAN_Transmit(CAN1, &CanTxData);
    }
}

void SIMULATOR_UpdatePower(void) {
    static float tmpCurrent, tmpVoltage;
    static float ratio;

    VoltageConversion();
    CurrentConversion();
    ++SampleCount;

    if (SampleCount != 19) return;

    tmpCurrent = tmpVoltage = 0.0f;
    for (uint8_t i = 0; i < 20; ++i) {
        tmpCurrent += SIMULATOR_CurrentBuffer[i];
        tmpVoltage += SIMULATOR_VoltageBuffer[i];
    }
    SIMULATOR_Data.current = tmpCurrent/20.0f;
    SIMULATOR_Data.voltage = tmpVoltage/20.0f;
    SIMULATOR_Data.power = SIMULATOR_Data.current * SIMULATOR_Data.voltage;

    SIMULATOR_Data.remainEnergy = SIMULATOR_Data.remainEnergy -
        (SIMULATOR_Data.power - CHASSIS_MAX_POWER) * 0.02f;
    if (SIMULATOR_Data.remainEnergy > 60.0f)
        SIMULATOR_Data.remainEnergy = 60.0f;
    else if (SIMULATOR_Data.remainEnergy < 0.0f) {
        SIMULATOR_Data.remainEnergy = 0.0f;
        ratio = (SIMULATOR_Data.power - CHASSIS_MAX_POWER) / CHASSIS_MAX_POWER;
        if (ratio <= 0.1f)
            SIMULATOR_Data.remainLife -= 3;
        else if (ratio <= 0.2f)
            SIMULATOR_Data.remainLife -= 6;
        else
            SIMULATOR_Data.remainLife -= 12;

        if (SIMULATOR_Data.remainLife < 0)
            SIMULATOR_Data.remainLife = 0;
    }
    SampleCount = 0;

    /* send info data */
    SIMULATOR_InfoData.remainLifeValue = SIMULATOR_Data.remainLife;
    SIMULATOR_InfoData.realChassisOutV = SIMULATOR_Data.voltage;
    SIMULATOR_InfoData.realChassisOutA = SIMULATOR_Data.current;
    MONITOR_Send((uint8_t*)&SIMULATOR_InfoData, sizeof(SIMULATOR_InfoData));
}

void SIMULATOR_Hit(void) {
    SIMULATOR_Data.remainLife -= 50;
    if (SIMULATOR_Data.remainLife < 0)
        SIMULATOR_Data.remainLife = 0;
}
