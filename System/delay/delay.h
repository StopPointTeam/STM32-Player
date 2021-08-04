//SYSCLK 初始化及延时函数

#ifndef __DELAY_H
#define __DELAY_H

#include "sys.h"

void SYSCLK_Init(uint16_t SYSCLK);
void Delay_us(uint32_t nus);
void Delay_ms(uint16_t nms);

#endif
