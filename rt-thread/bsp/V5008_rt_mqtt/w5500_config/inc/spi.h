#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"

void WIZ_SPI_Init(void);
void WIZ_CS(uint8_t val);
uint8_t SPI1_SendByte(uint8_t byte);
#endif

