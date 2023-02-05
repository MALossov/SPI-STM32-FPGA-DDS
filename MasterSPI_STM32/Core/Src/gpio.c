/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "Rotary_Encoder.h"
#include "usart.h"
#include "stdio.h"
#include "ssd1306.h"
#include "menu.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, OLED_SCK_Pin|OLED_SDA_Pin|OLED_RST_Pin|OLED_DC_Pin
                          |SPI1_NSS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin */
  GPIO_InitStruct.Pin = OLED_SCK_Pin|OLED_SDA_Pin|OLED_RST_Pin|OLED_DC_Pin
                          |SPI1_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = Encoder_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Encoder_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = Encoder_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Encoder_B_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = Encoder_S_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Encoder_S_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SW_S1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW_S1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SW_S2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW_S2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 2 */
// 旋转编码器结构体，用于存储旋转编码器的状�?
typedef struct {
  enum B_Pin_Set { B_Low, B_High } A_Rising;
  enum B_Pin_Set A_Falling;
  enum A_Pin_Stat { A_Rised, A_Finished, EncIDLE } A_State;

}Encoder_TypeDef;
Encoder_TypeDef Encoder = { B_Low, B_High, EncIDLE };

// 引入菜单控制
extern MenuList Menu;


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == EC11_A_GPIO_PIN) {
    if (!(Encoder.A_State == EncIDLE || Encoder.A_State == A_Finished))
    {
      Encoder.A_State = EncIDLE;
      return;
    }

    if (HAL_GPIO_ReadPin(EC11_B_GPIO_Group, EC11_B_GPIO_PIN) == GPIO_PIN_SET)
    {
      Encoder.A_State = A_Rised;
      Encoder.A_Rising = B_High;
    }
    else
    {
      Encoder.A_Rising = B_Low;
      Encoder.A_State = A_Rised;

    }
  }
  else if (GPIO_Pin == SW_S2_Pin) {
    Menu.switchFlg = 1;
    Menu.Switches.SW2 = Switch_Pressed;
  }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == SW_S1_Pin) {
    Menu.switchFlg = 1;
    Menu.Switches.SW1 = Switch_Pressed;
  }
  else if (GPIO_Pin == Encoder_S_Pin) {
    Menu.switchFlg = 1;
    Menu.Switches.ESW = Switch_Pressed;
  }
  else if (GPIO_Pin == EC11_A_GPIO_PIN) {
    Encoder.A_Falling = (enum B_Pin_Set)(HAL_GPIO_ReadPin(Encoder_B_GPIO_Port, Encoder_B_Pin));
    if (Encoder.A_Falling == B_Low && Encoder.A_Rising == B_High && Encoder.A_State == A_Rised)
    {
      Encoder.A_State = A_Finished;
      Menu.encFlg = 1;
      Menu.Switches.Encoder = AntiClockWise;
    }
    else if (Encoder.A_Falling == B_High && Encoder.A_Rising == B_Low && Encoder.A_State == A_Rised)
    {
      Encoder.A_State = A_Finished;
      Menu.encFlg = 1;
      Menu.Switches.Encoder = ClockWise;
    }
    else
    {
      Encoder.A_State = EncIDLE;
    }
  }

}
/* USER CODE END 2 */
