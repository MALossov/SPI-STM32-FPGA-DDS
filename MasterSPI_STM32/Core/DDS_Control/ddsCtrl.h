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
    uint8_t amp;
    WaveList wave;
} DDS_Wave;


#endif

