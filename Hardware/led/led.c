//LED 库

#include "sys.h"

#include "led.h"

/**
  * @brief  LED 初始化，用于 LED2
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE(); //开启 PA 时钟

    GPIO_Initure.Pin = GPIO_PIN_1;                  //PA1
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;        //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;                //上拉
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //高速
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);            //初始化 PA1

    LED2_Off;
    SysTick_StartTimer(LED2_SYSTICK_TIMER_ID, 50);
}

/**
  * @brief  LED2 慢速翻转，无阻塞，防止因调用过快导致无法观察
  * @param  led_num: 序号
  */
void LED_Slow_Toggle(uint8_t led_num)
{
    switch (led_num)
    {
    case 2:
    {
        if (SysTick_CheckTimer(LED2_SYSTICK_TIMER_ID))
        {
            HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
            SysTick_StartTimer(LED2_SYSTICK_TIMER_ID, 50);
        }
    }
    }
}
