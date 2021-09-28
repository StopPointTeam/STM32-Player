//ϵͳ���õ���

#ifndef __SYS_H
#define __SYS_H

#include "stm32h7xx.h"
#include "core_cm7.h"
#include "stm32h7xx_hal.h"

//����ȫ���жϵĺ�
#define ENABLE_INT() __set_PRIMASK(0)  //ʹ��ȫ���ж�
#define DISABLE_INT() __set_PRIMASK(1) //��ֹȫ���ж�

void MPU_Config(void);
void Cache_Enable(void);
void SystemClock_Init(void);
uint8_t Get_ICacheSta(void);
uint8_t Get_DCacheSta(void);

void Error_Handler(char *file, uint32_t line);

#endif
