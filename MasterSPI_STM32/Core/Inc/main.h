/* USER CODE BEGIN Header */
 /**
   ******************************************************************************
   * @file           : main.h
   * @brief          : Header for main.c file.
   *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_SCK_Pin GPIO_PIN_0
#define OLED_SCK_GPIO_Port GPIOA
#define OLED_SDA_Pin GPIO_PIN_4
#define OLED_SDA_GPIO_Port GPIOA
#define OLED_RST_Pin GPIO_PIN_5
#define OLED_RST_GPIO_Port GPIOA
#define PWM_Out_Pin GPIO_PIN_6
#define PWM_Out_GPIO_Port GPIOA
#define Encoder_A_Pin GPIO_PIN_0
#define Encoder_A_GPIO_Port GPIOB
#define Encoder_A_EXTI_IRQn EXTI0_1_IRQn
#define Encoder_B_Pin GPIO_PIN_1
#define Encoder_B_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_8
#define OLED_DC_GPIO_Port GPIOA
#define Encoder_S_Pin GPIO_PIN_6
#define Encoder_S_GPIO_Port GPIOC
#define Encoder_S_EXTI_IRQn EXTI4_15_IRQn
#define SW_S1_Pin GPIO_PIN_11
#define SW_S1_GPIO_Port GPIOA
#define SW_S1_EXTI_IRQn EXTI4_15_IRQn
#define SW_S2_Pin GPIO_PIN_12
#define SW_S2_GPIO_Port GPIOA
#define SW_S2_EXTI_IRQn EXTI4_15_IRQn
#define SPI1_NSS_Pin GPIO_PIN_15
#define SPI1_NSS_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
