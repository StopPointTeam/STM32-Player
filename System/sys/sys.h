//系统常用调用

#ifndef __SYS_H
#define __SYS_H

#include "stm32h7xx.h"
#include "core_cm7.h"
#include "stm32h7xx_hal.h"

//开关全局中断的宏
#define ENABLE_INT() __set_PRIMASK(0)  //使能全局中断
#define DISABLE_INT() __set_PRIMASK(1) //禁止全局中断

void MPU_Config(void);
void Cache_Enable(void);
void SystemClock_Init(void);
uint8_t Get_ICacheSta(void);
uint8_t Get_DCacheSta(void);

void Error_Handler(char *file, uint32_t line);

#endif
