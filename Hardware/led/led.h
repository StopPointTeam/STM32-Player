//LED 库

#ifndef _LED_H
#define _LED_H

#include "sys.h"

#define LED2(n) (n ? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET))
#define LED2_On LED2(0)
#define LED2_Off LED2(1)
#define LED2_Toggle HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1) //LED2 翻转
#define LED2_SYSTICK_TIMER_ID 0
#define LED2_Slow_Toggle LED_Slow_Toggle(2) //LED2 慢速翻转，无阻塞，防止因调用过快导致无法观察

void LED_Init(void);
void LED_Slow_Toggle(uint8_t led_num);

#endif
