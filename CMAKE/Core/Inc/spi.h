#ifndef __SPI_H
#define __SPI_H
#include "stm32f1xx.h"
#include "main.h"

extern SPI_HandleTypeDef SPI1_Handler; 

void SPI1_Init(void);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI1_ReadWriteByte(u8 TxData);
#endif
