/*
 * @Description:
 * @Author: MALossov
 * @Date: 2023-02-05 23:22:28
 * @LastEditTime: 2023-02-06 14:37:06
 * @LastEditors: MALossov
 * @Reference:
 */
#include "spi_lib.h"
#include "spi_host.h"

int TestSpiFpga(void)
{
   HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
   uint8_t no_param_test[3] = { 0x0, 0x0, 0x0 };
   uint8_t spi_status_test = 0;
   uint8_t data_read_test[3];
   uint8_t val_inv_test[3] = { 0x38, 0xAE, 0x3B };
   uint8_t val_led_yellow_test[3] = { 0x0, 0x0, 0x3 };
   uint8_t val_led_blue_test[3] = { 0x0, 0x0, 0x4 };

   spi_send(SPI_INIT, no_param_test, NULL);

   spi_send(SPI_SEND_BIT_INV, val_inv_test, &spi_status_test); // send values bit inversion
   printf("send inversion data, status: 0x%x\n", spi_status_test);

   spi_send(SPI_READ_REQ_BIT_INV, no_param_test, NULL); //send read request
   spi_read(data_read_test, &spi_status_test); // read data inversion

   for (size_t i = 0; i < 3; i++) {
      printf("bit inversion read idx %i: 0x%x, should be 0x%x\n", i, data_read_test[i], 0xFF & ~val_inv_test[i]);
   }
   printf("status: 0x%x\n", spi_status_test);

//   spi_send(SPI_SET_LED, val_led_yellow_test, &spi_status_test); // led yellow
//   printf("send yellow led, status: 0x%x\n", spi_status_test);

//   spi_send(SPI_READ_REQ_LED, no_param_test, NULL); //send led read request

//   spi_read(data_read_test, &spi_status_test); // read led data
//   printf("led_data read: 0x%x, 0x%x, 0x%x, status:0x%x\n", data_read_test[2], data_read_test[1], data_read_test[0], spi_status_test);

   //wait 2sec before setting led in blue
//   HAL_Delay(2000);

//   spi_send(SPI_SET_LED, val_led_blue_test, &spi_status_test); // set led blue
//   printf("send blue led, status: 0x%x\n", spi_status_test);

   //send 4 values the fastest possible
   for (size_t i = 0; i < 4; i++) {
      int send_value = (i + 1) * 16;
      spi_send24b(SPI_SEND_VEC, send_value, &spi_status_test);
      printf("sent vector val: 0x%x, status: 0x%x\n", send_value, spi_status_test);
   }

   //send read request, the fpga will send the 4 values
   

   //read values the fastest possible
	 HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	 spi_send(SPI_INIT, no_param_test, NULL);
	 spi_send(SPI_READ_VEC, no_param_test, &spi_status_test);
   printf("sent read req vector, status: 0x%x\n", spi_status_test);
   for (size_t i = 0; i < 4; i++) {
      spi_read(data_read_test, &spi_status_test);
      printf("vector read: 0x%x, 0x%x, 0x%x, status:0x%x\n", data_read_test[2], data_read_test[1], data_read_test[0], spi_status_test);
   }

   HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

   return 0;
}

