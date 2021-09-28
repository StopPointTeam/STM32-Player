//LED ��

#ifndef _LED_H
#define _LED_H

#include "sys.h"

#define LED2(n) (n ? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET))
#define LED2_On LED2(0)
#define LED2_Off LED2(1)
#define LED2_Toggle HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1) //LED2 ��ת
#define LED2_SYSTICK_TIMER_ID 0
#define LED2_Slow_Toggle LED_Slow_Toggle(2) //LED2 ���ٷ�ת������������ֹ����ù��쵼���޷��۲�

void LED_Init(void);
void LED_Slow_Toggle(uint8_t led_num);

#endif
