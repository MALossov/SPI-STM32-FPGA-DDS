#include "main.h"
#include "spi_lib.h"

#ifndef __DDS_CTRL_H
#define __DDS_CTRL_H

typedef enum WaveList {
    Sine,
    Square,
    Triangle
} WaveList;

typedef struct DDS_Wave {
    uint32_t freq;
    uint32_t amp;
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

