//SysTick 及软件定时器库

#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "sys.h"

#define TMR_COUNT 4 //软件定时器的个数

//定时器结构体工作模式
typedef enum
{
    TMR_ONCE_MODE = 0, //一次工作模式
    TMR_AUTO_MODE = 1  //自动定时工作模式
} TMR_MODE_E;

//定时器结构体
typedef struct
{
    volatile uint8_t Mode;     //计数器模式
    volatile uint8_t Flag;     //定时到达标志
    volatile uint32_t Count;   //计数器
    volatile uint32_t PreLoad; //计数器预装值
} SOFT_TMR;

void SysTick_Init(void);
void Delay_ms(uint32_t n);
void Delay_us(uint32_t n);
void SysTick_StartTimer(uint8_t _id, uint32_t _period);
void SysTick_StartAutoTimer(uint8_t _id, uint32_t _period);
void SysTick_StopTimer(uint8_t _id);
uint8_t SysTick_CheckTimer(uint8_t _id);
int32_t SysTick_GetRunTime(void);
int32_t SysTick_CheckRunTime(int32_t _LastTime);

#endif
