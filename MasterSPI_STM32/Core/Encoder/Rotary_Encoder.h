#ifndef _ROTARY_ENCODER_
#define _ROTARY_ENCODER_
#include "main.h"
/*
需要：
    1.外部中断（A相）
        上升沿触发中断
    2.GPIO（B相）
        上拉输入模式
 */
 //外部中断
#define EC11_A_GPIO_Group Encoder_A_GPIO_Port
#define EC11_A_GPIO_PIN Encoder_A_Pin

// GPIO输入
#define EC11_B_GPIO_Group Encoder_B_GPIO_Port
#define EC11_B_GPIO_PIN Encoder_B_Pin
void EC11_Decode(void (*Clockwise)(void), void (*Anticlockwise)(void));
void Encoder_Clockwise(void);
void Encoder_Anticlockwise(void);

#endif
