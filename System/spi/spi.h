//基于 HAL 库封装的 SPI 操作函数

#ifndef __SPI_H
#define __SPI_H

#include "sys.h"

extern SPI_HandleTypeDef SPI2_Handler;

void SPI2_Init(void);
void SPI2_SetSpeed(uint32_t SPI_BaudRatePrescaler);
uint8_t SPI2_ReadWrite8Bits(uint8_t TxData);
void SPI2_ReadWriteBytes(uint8_t *pTData, uint8_t *pRData, uint32_t Count);
void SPI2_Write8Bits(uint8_t Data);
void SPI2_Write16Bits(uint16_t Data);
void SPI2_Write32Bits(uint32_t Data);
void SPI2_WriteBytes(uint8_t *pData, uint32_t Count);
uint8_t SPI2_Read8Bits(void);
void SPI2_ReadBytes(uint8_t *pData, uint32_t Count);

#endif
