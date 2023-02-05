/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-05 11:01:27
 * @LastEditTime: 2023-02-05 23:03:24
 * @LastEditors: MALossov
 * @Reference:
 */
#include "ddsCtrl.h"

#ifndef __MENU_H
#define __MENU_H

#define TITLE_OFFSET 50

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
    Action SwitchAction;
}SwitchController;

typedef struct LabelText {
    char* Title;
    uint8_t pos_x, pos_y;
}LabelText;

typedef struct DataText {
    char* Data;
    uint8_t pos_x, pos_y;
    char* appendex;
}DataText;


typedef char WaveNames;
typedef uint8_t flagType;

typedef enum {
    WaveFormat,
    Frequency,
    Steplint,
    Amplitude
} ChoosenBar;

typedef enum {
    DDS_Play,
    DDS_Pause,
    DDS_Menu_Modifying
}DDS_State;

typedef struct MenuList {
    DDS_Wave WaveForm;
    SwitchController Switches;
    LabelText Label[5];
    DataText Data[4];
    flagType switchFlg, encFlg, Inited;
    ChoosenBar Choose;
    DDS_State DDS_STATE;
} MenuList;


typedef enum StatusChoose {
    Choose,
    UnChoose
}StatusChoose;

// Functions
void Menu_Init(void);
void callBackingBtns(void);

void update2Data(DataText* Data, uint32_t value, char* appendex, uint8_t row);
void updateStatusChoose(uint8_t row, StatusChoose status);
void updateSTATE(DDS_State DDSSTATE);
void update2Wave(DataText* WavData, WaveList Wav);

#endif

