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
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
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
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == EC11_A_GPIO_PIN) {
    
    printf("GPIO_Pin == EC11_A_GPIO_PIN\n");
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    if (HAL_GPIO_ReadPin(EC11_B_GPIO_Group, EC11_B_GPIO_PIN) == GPIO_PIN_SET)
    {
        Encoder_Anticlockwise();
    }
    else
    {
        Encoder_Clockwise();
    }
  }
  else if (GPIO_Pin == SW_S2_Pin) {
    printf("GPIO_Pin == SW_S2_Pin\n");
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		ssd1306_Fill(Black);
		ssd1306_SetCursor(25, 53);
		ssd1306_FillRectangle(0,51,128,64,White);
		ssd1306_WriteString("SW2 Pressed", Font_7x10, Black);
		HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port,SPI1_NSS_Pin,GPIO_PIN_RESET);
    ssd1306_UpdateScreen();
		ssd1306_UpdateScreen();
  }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == SW_S1_Pin) {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    printf("GPIO_Pin == SW_S1_Pin\n");
    ssd1306_Fill(Black);
    ssd1306_SetCursor(25, 53);
		ssd1306_FillRectangle(0,51,128,64,White);
    ssd1306_WriteString("SW1 Pressed", Font_7x10, Black);
		HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port,SPI1_NSS_Pin,GPIO_PIN_SET);
    ssd1306_UpdateScreen();
  }else if(GPIO_Pin == Encoder_S_Pin){
		 HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    printf("GPIO_Pin == Encoder_S_Pin\n");
    ssd1306_Fill(Black);
    ssd1306_SetCursor(15, 53);
		ssd1306_FillRectangle(0,51,128,64,White);
    ssd1306_WriteString("Encoder Pressed", Font_7x10, Black);
		HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port,SPI1_NSS_Pin,GPIO_PIN_SET);
    ssd1306_UpdateScreen();
	}

}
/* USER CODE END 2 */
