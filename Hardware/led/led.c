#include "gpio.h"

#include "led.h"

/**
  * @brief  LED ��ʼ�������� LED2
  */
void LED_Init(void)
{
    GPIO_A1_Init();
    LED2_Off;
}
