/*
 * @Description: spi_lib.c
 * @Author: MALossov
 * @Date: 2023-02-02 17:01:59
 * @LastEditTime: 2023-02-02 22:19:00
 * @LastEditors: MALossov
 * @Reference:
 */
#include "spi_lib.h"
#include <stdio.h>

int spi_send(uint8_t cmd, uint8_t val[3], uint8_t* status) {
  uint8_t to_send[] = { cmd, val[0], val[1], val[2] };
  uint8_t recv[4] = { 0, 0, 0, 0 };
  uint8_t status_recv = 0;
  uint32_t retries = 0;
	
	//uint8_t MASK = (cmd == SPI_INIT)?(STATUS_FPGA_WAKE_MASK):(STATUS_FPGA_RECV_MASK);

	do{
  HAL_SPI_TransmitReceive(&hspi1, to_send, recv, 4, 1000);
	status_recv = recv[0];
		retries++;
//		printf("Retries: %d, Status: %d\t|",retries,status_recv);
	}while(retries < 100 && (status_recv & STATUS_FPGA_RECV_MASK) == 0);
//	printf("\n");
	
	
  status_recv = recv[0];

  if (status != NULL) {
    *status = status_recv;
  }
  return retries >= 100;
}

int spi_send3(uint8_t cmd, uint8_t val0, uint8_t val1, uint8_t val2,
  uint8_t* status) {
  uint8_t param[3] = { val0, val1, val2 };
  return spi_send(cmd, param, status);
}

int spi_send24b(uint8_t cmd, uint32_t val24b, uint8_t* status) {
  uint8_t param[3] = { val24b & 0xff, (val24b >> 8) & 0xff,
                      (val24b >> 16) & 0xff };
  return spi_send(cmd, param, status);
}

int spi_read(uint8_t val[3], uint8_t* status) {
  uint8_t nop_command[] = { 0x00, 0x00, 0x00, 0x00 }; // nop
  uint8_t status_recv = 0;
  uint32_t retries = 0;
  uint32_t max_retries = 1000;

	
	do{
		HAL_SPI_TransmitReceive(&hspi1, nop_command, val, 4, 1000);
		retries++;
		status_recv = val[0];
//		printf("Retries: %d, Status: %d\t|",retries,status_recv);
	}while(retries < max_retries && (status_recv & STATUS_FPGA_SEND_MASK) == 0);
//	printf("\n");
  status_recv = val[0];
	
	val[0]=val[1];
	val[1]=val[2];
	val[2]=val[3];

  if (status != NULL) {
    *status = status_recv;
  }

  return retries < max_retries;
}
