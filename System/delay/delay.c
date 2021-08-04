//SYSCLK ��ʼ������ʱ����

#include "delay.h"

static uint32_t fac_us = 0; //us ��ʱ������

/**
  * @brief  ��ʼ���ӳٺ���
  * @param  SYSCLK: ϵͳʱ��Ƶ��
  */
void SYSCLK_Init(uint16_t SYSCLK)
{

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    fac_us = SYSCLK;
}

/**
  * @brief  us �ӳٺ���
  * @param  nus: us �������ɳ��� 1000
  */
void Delay_us(uint32_t nus)
{
    uint32_t ticks, told, tnow;
    uint32_t tcnt = 0;
    uint32_t reload = SysTick->LOAD; //LOAD ��ֵ
    ticks = nus * fac_us;            //��Ҫ�Ľ�����
    told = SysTick->VAL;             //��ǰ������ֵ

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
  * @brief  ms �ӳٺ���
  * @param  nms: ms ��
  */
void Delay_ms(uint16_t nms)
{
    uint32_t i;
    for (i = 0; i < nms; i++)
        Delay_us(1000);
}
