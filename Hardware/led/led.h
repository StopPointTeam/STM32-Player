#ifndef _LED_H
#define _LED_H

#include "sys.h"

#define LED2(n) (n ? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET))
#define LED2_On LED2(0)
#define LED2_Off LED2(1)
#define LED2_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1)) //LED2 ·­×ª

void LED_Init(void);

#endif
