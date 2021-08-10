//系统初始化

#ifndef __SYS_H
#define __SYS_H

#include "stm32h7xx.h"
#include "core_cm7.h"
#include "stm32h7xx_hal.h"

#define Write_Through() (*(__IO uint32_t *)0XE000EF9C = 1UL << 2) //Cache 透写模式

void MPU_Config(void);
void Cache_Enable(void);
uint8_t Clock_Init(uint32_t plln, uint32_t pllm, uint32_t pllp, uint32_t pllq);
uint8_t Get_ICacheSta(void);
uint8_t Get_DCacheSta(void);
void QSPI_Enable_Memmapmode(void);

#if defined(__clang__)
void __attribute__((noinline)) WFI_SET(void);
void __attribute__((noinline)) INTX_DISABLE(void);
void __attribute__((noinline)) INTX_ENABLE(void);
void __attribute__((noinline)) MSR_MSP(uint32_t addr);
#elif defined(__CC_ARM)
void WFI_SET(void);
void INTX_DISABLE(void);
void INTX_ENABLE(void);
void MSR_MSP(uint32_t addr);
#endif

#endif
