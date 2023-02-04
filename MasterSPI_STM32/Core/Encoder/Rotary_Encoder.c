/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-02 11:26:32
 * @LastEditTime: 2023-02-02 16:14:06
 * @LastEditors: MALossov
 * @Reference:
 */
#include "Rotary_Encoder.h"
#include "ssd1306.h"
#include "stdio.h"
#include "usart.h"

 /**
  * @brief 旋转编码器解码函数（A相的外部中断调用）
  * @param Clockwise 顺时钟时的回调函数
  * @param Anticlockwise 逆时针时的回调函数
  * @return 无
  * @author HZ12138
  * @date 2022-07-12 15:45:59
  */

void Encoder_Clockwise(void) {
    // 顺时针
    ssd1306_Fill(Black);
	ssd1306_FillRectangle(0,51,128,64,White);
    ssd1306_SetCursor(30, 53);
    ssd1306_WriteString("Clockwise", Font_7x10, Black);
    ssd1306_UpdateScreen();
    printf("Clockwise!\n");
}

void Encoder_Anticlockwise(void) {
    // 逆时针
    ssd1306_Fill(Black);
	ssd1306_FillRectangle(0,51,128,64,White);
    ssd1306_SetCursor(20, 53);
    ssd1306_WriteString("Anticlockwise", Font_7x10, Black);
    ssd1306_UpdateScreen();
    printf("Anticlockwise\n");
}

