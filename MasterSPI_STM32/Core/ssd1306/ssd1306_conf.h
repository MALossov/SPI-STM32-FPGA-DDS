/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-02 11:36:44
 * @LastEditTime: 2023-02-02 12:14:32
 * @LastEditors: MALossov
 * @Reference:
 */
 /**
  * Private configuration file for the SSD1306 library.
  * This example is configured for STM32F0, I2C and including all fonts.
  */

#ifndef __SSD1306_CONF_H__
#define __SSD1306_CONF_H__

#include "main.h"

  // Choose a microcontroller family
//#define STM32F0
//#define STM32F1
//#define STM32F4
//#define STM32L0
//#define STM32L1
//#define STM32L4
//#define STM32F3
//#define STM32H7
//#define STM32F7
#define STM32G0

// Choose a bus
// #define SSD1306_USE_I2C
// #define SSD1306_USE_SPI
//#define SSD1306_USE_SOFT_I2C
#define SSD1306_USE_SOFT_SPI 

#ifdef SSD1306_USE_SOFT_I2C
// Soft I2C Configuration
//#define SSD1306_I2C_SDA_Port        OLED_SDA_GPIO_Port
//#define SSD1306_I2C_SDA_Pin         OLED_SDA_Pin
//#define SSD1306_I2C_SCL_Port        OLED_SCK_GPIO_Port
//#define SSD1306_I2C_SCL_Pin         OLED_SCK_Pin
//#define SSD1306_I2C_ADDR        (0x3C << 1)
#elif defined(SSD1306_USE_I2C)

// I2C Configuration
//#define SSD1306_I2C_PORT        hi2c1
//#define SSD1306_I2C_ADDR        (0x3C << 1)
#elif defined(SSD1306_USE_SPI)
// SPI Configuration
//#define SSD1306_SPI_PORT        hspi1
//#define SSD1306_CS_Port         OLED_CS_GPIO_Port
//#define SSD1306_CS_Pin          OLED_CS_Pin
//#define SSD1306_DC_Port         OLED_DC_GPIO_Port
//#define SSD1306_DC_Pin          OLED_DC_Pin
//#define SSD1306_Reset_Port      OLED_RST_GPIO_Port
//#define SSD1306_Reset_Pin       OLED_RST_Pin
#elif defined(SSD1306_USE_SOFT_SPI)
// SOFTWARE SPI Configuration
#define SSD1306_SCLK_Port       OLED_SCK_GPIO_Port
#define SSD1306_SCLK_Pin        OLED_SCK_Pin
#define SSD1306_MOSI_Port       OLED_SDA_GPIO_Port
#define SSD1306_MOSI_Pin        OLED_SDA_Pin
#define SSD1306_CS_Port         OLED_CS_GPIO_Port
#define SSD1306_CS_Pin          OLED_CS_Pin
#define SSD1306_DC_Port         OLED_DC_GPIO_Port
#define SSD1306_DC_Pin          OLED_DC_Pin
#define SSD1306_Reset_Port      OLED_RST_GPIO_Port
#define SSD1306_Reset_Pin       OLED_RST_Pin
#endif

// Mirror the screen if needed
// #define SSD1306_MIRROR_VERT
// #define SSD1306_MIRROR_HORIZ

// Set inverse color if needed
// # define SSD1306_INVERSE_COLOR

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6x8
#define SSD1306_INCLUDE_FONT_7x10
#define SSD1306_INCLUDE_FONT_11x18
#define SSD1306_INCLUDE_FONT_16x26

#define SSD1306_INCLUDE_FONT_16x24

// The width of the screen can be set using this
// define. The default value is 128.
// #define SSD1306_WIDTH           64

// If your screen horizontal axis does not start
// in column 0 you can use this define to
// adjust the horizontal offset
// #define SSD1306_X_OFFSET

// The height can be changed as well if necessary.
// It can be 32, 64 or 128. The default value is 64.
// #define SSD1306_HEIGHT          64

#endif /* __SSD1306_CONF_H__ */
