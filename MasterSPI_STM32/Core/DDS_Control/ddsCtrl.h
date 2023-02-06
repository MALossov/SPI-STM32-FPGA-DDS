/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-05 11:07:30
 * @LastEditTime: 2023-02-06 19:14:36
 * @LastEditors: MALossov
 * @Reference:
 */
#include "main.h"

#ifndef __DDS_CTRL_H
#define __DDS_CTRL_H

typedef enum WaveList {
    Sine = 1,
    Square = 2,
    Triangle = 3,
} WaveList;

typedef struct DDS_Wave {
    uint32_t freq;
    uint16_t amp;
    WaveList wave;
    uint32_t step;
} DDS_Wave;

typedef enum Action {
    Increase,
    Decrease,
    PauseOrPlay,
    Reset,
    ChangeSelection,
    Waiting
} Action;

void ModifieDDS(void);

#endif

