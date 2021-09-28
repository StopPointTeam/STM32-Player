//硬件定时器库

#include "sys.h"

#include "tim.h"

/**
  * @brief  开启定时中断
  * @param  TIMx: 定时器
  * @param  _ulFreq: 定时频率 (Hz)。0 表示关闭
  * @param  _PreemptionPriority: 抢占优先级
  * @param  _SubPriority: 子优先级
  */
void TIM_Set(TIM_TypeDef *TIMx, double _ulFreq, uint8_t _PreemptionPriority, uint8_t _SubPriority)
{
    TIM_HandleTypeDef TimHandle = {0};
    uint16_t usPeriod;
    uint16_t usPrescaler;
    uint32_t uiTIMxCLK;

    //使能TIM时钟
    if (TIMx == TIM1)
        __HAL_RCC_TIM1_CLK_ENABLE();
    else if (TIMx == TIM2)
        __HAL_RCC_TIM2_CLK_ENABLE();
    else if (TIMx == TIM3)
        __HAL_RCC_TIM3_CLK_ENABLE();
    else if (TIMx == TIM4)
        __HAL_RCC_TIM4_CLK_ENABLE();
    else if (TIMx == TIM5)
        __HAL_RCC_TIM5_CLK_ENABLE();
    else if (TIMx == TIM6)
        __HAL_RCC_TIM6_CLK_ENABLE();
    else if (TIMx == TIM7)
        __HAL_RCC_TIM7_CLK_ENABLE();
    else if (TIMx == TIM8)
        __HAL_RCC_TIM8_CLK_ENABLE();
    else if (TIMx == TIM12)
        __HAL_RCC_TIM12_CLK_ENABLE();
    else if (TIMx == TIM13)
        __HAL_RCC_TIM13_CLK_ENABLE();
    else if (TIMx == TIM14)
        __HAL_RCC_TIM14_CLK_ENABLE();
    else if (TIMx == TIM15)
        __HAL_RCC_TIM15_CLK_ENABLE();
    else if (TIMx == TIM16)
        __HAL_RCC_TIM16_CLK_ENABLE();
    else if (TIMx == TIM17)
        __HAL_RCC_TIM17_CLK_ENABLE();

    if ((TIMx == TIM1) || (TIMx == TIM8) || (TIMx == TIM15) || (TIMx == TIM16) || (TIMx == TIM17))
        uiTIMxCLK = SystemCoreClock / 2; //APB2 定时器时钟 = 200M
    else
        uiTIMxCLK = SystemCoreClock / 2; //APB1 定时器 = 200M

    if (_ulFreq < 100)
    {
        usPrescaler = 10000 - 1;                                  //分频比 = 10000
        usPeriod = ((double)uiTIMxCLK / 10000.0) / _ulFreq - 1.0; //自动重装的值
    }
    else if (_ulFreq < 3000)
    {
        usPrescaler = 100 - 1;                                  //分频比 = 100
        usPeriod = ((double)uiTIMxCLK / 100.0) / _ulFreq - 1.0; //自动重装的值
    }
    else //大于 4k 的频率
    {
        usPrescaler = 0;                              //分频比 = 1
        usPeriod = (double)uiTIMxCLK / _ulFreq - 1.0; //自动重装的值
    }

    //定时器中断更新周期
    TimHandle.Instance = TIMx;
    TimHandle.Init.Prescaler = usPrescaler;
    TimHandle.Init.Period = usPeriod;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.RepetitionCounter = 0;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&TimHandle);

    //使能定时器中断
    __HAL_TIM_ENABLE_IT(&TimHandle, TIM_IT_UPDATE);

    //配置TIM定时更新中断
    {
        uint8_t irq = 0;

        if (TIMx == TIM1)
            irq = TIM1_UP_IRQn;
        else if (TIMx == TIM2)
            irq = TIM2_IRQn;
        else if (TIMx == TIM3)
            irq = TIM3_IRQn;
        else if (TIMx == TIM4)
            irq = TIM4_IRQn;
        else if (TIMx == TIM5)
            irq = TIM5_IRQn;
        else if (TIMx == TIM6)
            irq = TIM6_DAC_IRQn;
        else if (TIMx == TIM7)
            irq = TIM7_IRQn;
        else if (TIMx == TIM8)
            irq = TIM8_UP_TIM13_IRQn;
        else if (TIMx == TIM12)
            irq = TIM8_BRK_TIM12_IRQn;
        else if (TIMx == TIM13)
            irq = TIM8_UP_TIM13_IRQn;
        else if (TIMx == TIM14)
            irq = TIM8_TRG_COM_TIM14_IRQn;
        else if (TIMx == TIM15)
            irq = TIM15_IRQn;
        else if (TIMx == TIM16)
            irq = TIM16_IRQn;
        else if (TIMx == TIM17)
            irq = TIM17_IRQn;

        HAL_NVIC_SetPriority((IRQn_Type)irq, _PreemptionPriority, _SubPriority);
        HAL_NVIC_EnableIRQ((IRQn_Type)irq);
    }

    HAL_TIM_Base_Start(&TimHandle);
}
