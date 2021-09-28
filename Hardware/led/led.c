//LED ��

#include "sys.h"

#include "led.h"

/**
  * @brief  LED ��ʼ�������� LED2
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE(); //���� PA ʱ��

    GPIO_Initure.Pin = GPIO_PIN_1;                  //PA1
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;        //�������
    GPIO_Initure.Pull = GPIO_PULLUP;                //����
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; //����
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);            //��ʼ�� PA1

    LED2_Off;
    SysTick_StartTimer(LED2_SYSTICK_TIMER_ID, 50);
}

/**
  * @brief  LED2 ���ٷ�ת������������ֹ����ù��쵼���޷��۲�
  * @param  led_num: ���
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
