/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-05 11:01:27
 * @LastEditTime: 2023-02-05 11:11:56
 * @LastEditors: MALossov
 * @Reference:
 */
#include "ddsCtrl.h"

#ifndef __MENU_H
#define __MENU_H

typedef enum SwitchStatus {
    SWITCH_OFF,
    SWITCH_ON
} SwitchStatus;

typedef enum EncoderStatus {
    ClockWise,
    AntiClockWise
}EncoderStatus;

typedef struct SwitchController {
    SwitchStatus SW1, SW2, ESW;
    EncoderStatus Encoder;
}SwitchController;

typedef struct MenuList {
    DDS_Wave WaveForm;
    SwitchController Switches;
} MenuList;



#endif

