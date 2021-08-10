//SD �������⣬�����д�Ȳ�����ʹ�� FatFs �еĺ������

#ifndef __SD__H
#define __SD__H

#include "ff.h"

#define SD_OK 0
#define SD_ERROR 1

uint8_t SD_Init(void);
uint8_t SD_DeInit(void);

uint8_t SD_GetPath(uint8_t *filepath, uint8_t *filename);

uint8_t SD_SelectFile(uint8_t *filename, uint8_t *filesuffix);

#endif
