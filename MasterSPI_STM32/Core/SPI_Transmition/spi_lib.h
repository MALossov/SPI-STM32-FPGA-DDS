#include "spi.h"
#include "main.h"
#include "stdlib.h"

#ifndef SPI_LIB_H
#define SPI_LIB_H

#define STATUS_FPGA_RECV_OFFSET 6 //fpga has received data
#define STATUS_FPGA_SEND_OFFSET 7 //fpga has sent data
#define STATUS_FPGA_WAKE_OFFSET 5 //fpga has HAS WAKED

#define STATUS_FPGA_RECV_MASK (0x1<<STATUS_FPGA_RECV_OFFSET)
#define STATUS_FPGA_SEND_MASK (0x1<<STATUS_FPGA_SEND_OFFSET)
#define STATUS_FPGA_WAKE_MASK (0x1<<STATUS_FPGA_RECV_OFFSET)

#define SPI_NOP 0x00
#define SPI_INIT 0x01
#define SPI_SEND_BIT_INV 0x02
#define SPI_READ_REQ_BIT_INV 0x03
#define SPI_SET_LED 0x04
#define SPI_READ_REQ_LED 0x05
#define SPI_SEND_VEC 0x06
#define SPI_READ_VEC 0x07

int spi_send(uint8_t cmd, uint8_t val[3], uint8_t* status);
int spi_send3(uint8_t cmd, uint8_t val0, uint8_t val1, uint8_t val2, uint8_t* status);
int spi_send24b(uint8_t cmd, uint32_t val24b, uint8_t* status);

int spi_read(uint8_t val[3], uint8_t* status);

#endif
