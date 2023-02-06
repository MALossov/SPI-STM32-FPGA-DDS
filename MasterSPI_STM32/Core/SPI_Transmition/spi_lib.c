/*
 * @Description: spi_lib.c
 * @Author: MALossov
 * @Date: 2023-02-02 17:01:59
 * @LastEditTime: 2023-02-06 17:10:30
 * @LastEditors: MALossov
 * @Reference:
 */
#include "spi_lib.h"
#include <stdio.h>
#include "ddsCtrl.h"
#include "main.h"
#include "gpio.h"

extern uint8_t no_param[3];
extern uint8_t spi_status;
extern uint8_t data_read[3];
extern uint8_t val_inv[3];
extern uint8_t val_led_yellow[3];
extern uint8_t val_led_blue[3];
extern uint8_t val_led_red[3];
extern uint8_t val_led_green[3];

int spi_send(uint8_t cmd, uint8_t val[3], uint8_t* status) {
  uint8_t to_send[] = { cmd, val[0], val[1], val[2] };
  uint8_t recv[4] = { 0, 0, 0, 0 };
  uint8_t status_recv = 0;
  uint32_t retries = 0;

  //uint8_t MASK = (cmd == SPI_INIT)?(STATUS_FPGA_WAKE_MASK):(STATUS_FPGA_RECV_MASK);

  do {
    HAL_SPI_TransmitReceive(&hspi1, to_send, recv, 4, 1000);
    status_recv = recv[0];
    retries++;
    //		printf("Retries: %d, Status: %d\t|",retries,status_recv);
  } while (retries < 100 && (status_recv & STATUS_FPGA_RECV_MASK) == 0);
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


  do {
    HAL_SPI_TransmitReceive(&hspi1, nop_command, val, 4, 1000);
    retries++;
    status_recv = val[0];
    //		printf("Retries: %d, Status: %d\t|",retries,status_recv);
  } while (retries < max_retries && (status_recv & STATUS_FPGA_SEND_MASK) == 0);
  //	printf("\n");
  status_recv = val[0];

  val[0] = val[1];
  val[1] = val[2];
  val[2] = val[3];

  if (status != NULL) {
    *status = status_recv;
  }

  return retries < max_retries;
}

int spi_write_dds(uint32_t freq, uint16_t amp, WaveList wav) {
  uint8_t status = 0;
  int err = 0;
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	spi_send(SPI_INIT, no_param, NULL);
  err |= spi_send24b(SPI_WR_Freq, freq, &status);
  err |= spi_send24b(SPI_WR_AMPaWAV, ((uint32_t)((uint8_t)wav << 16 | amp)), &status);
  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

  return err;
}

void spi_dump_dds(void) {
	 HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	spi_send(SPI_INIT, no_param, NULL);
  spi_send(SPI_RD_DDS, no_param, &spi_status);
	uint32_t dds_rd[3];
  for (uint8_t i = 0; i < 4; i++) {
    spi_read(data_read, &spi_status);
		dds_rd[i] = (uint32_t)(data_read[2]<<16|data_read[1]<<8|data_read[0]);
  }
	printf("ReadDDS:freq: %d,wav: %d,amp: %d\n",dds_rd[0],dds_rd[1],dds_rd[2]);
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
}

