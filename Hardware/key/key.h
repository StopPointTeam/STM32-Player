//°´¼ü²Ù×÷

#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define NO_KEY 0
#define KEY1 1
#define KEY2 2

#define KEY1_Val HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)
#define KEY2_Val HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)

void KEY_Init(void);
uint8_t KEY_GetKey(void);
uint8_t KEY_GetKeyWait(void);

#endif
