//SYSCLK 初始化及延时函数

#include "delay.h"

static uint32_t fac_us = 0; //us 延时倍乘数

/**
  * @brief  初始化延迟函数
  * @param  SYSCLK: 系统时钟频率
  */
void SYSCLK_Init(uint16_t SYSCLK)
{

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    fac_us = SYSCLK;
}

/**
  * @brief  us 延迟函数
  * @param  nus: us 数，不可超过 1000
  */
void Delay_us(uint32_t nus)
{
    uint32_t ticks, told, tnow;
    uint32_t tcnt = 0;
    uint32_t reload = SysTick->LOAD; //LOAD 的值
    ticks = nus * fac_us;            //需要的节拍数
    told = SysTick->VAL;             //当前计数器值

    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break;
        }
    };
}

/**
  * @brief  ms 延迟函数
  * @param  nms: ms 数
  */
void Delay_ms(uint16_t nms)
{
    uint32_t i;
    for (i = 0; i < nms; i++)
        Delay_us(1000);
}
