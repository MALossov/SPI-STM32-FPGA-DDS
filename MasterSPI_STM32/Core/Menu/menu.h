/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-05 11:01:27
 * @LastEditTime: 2023-02-05 12:14:03
 * @LastEditors: MALossov
 * @Reference:
 */
#include "ddsCtrl.h"

#ifndef __MENU_H
#define __MENU_H

typedef enum EncoderAndSwitchStatus {
    IDLE,
    ClockWise,
    AntiClockWise,
    Switch_Pressed
}EncoderAndSwitchStatus;

typedef char ButtonMsg;

typedef struct SwitchController {
    EncoderAndSwitchStatus SW1, SW2, ESW, Encoder;
    ButtonMsg* ButtonMsgs[5];
}SwitchController;

typedef struct LabelText {
    char* Title;
    uint8_t pos_x, pos_y;
}LabelText;


typedef char WaveNames;
typedef uint8_t flagType;

typedef struct MenuList {
    DDS_Wave WaveForm;
    SwitchController Switches;
    LabelText Label[5];
    WaveNames* WaveList[3];
    flagType switchFlg, encFlg;
} MenuList;

// Functions
void Menu_Init(void);
void callBackingBtns(void);

#endif

