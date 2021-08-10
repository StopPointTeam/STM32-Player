//��������

#include "delay.h"
#include "gpio.h"

#include "key.h"

/**
  * @brief  KEY1 KEY2 ��ʼ������
  */
void KEY_Init(void)
{
    GPIO_E3_C5_Init();
}

/**
  * @brief  ��ȡ��ǰ���µļ����ȴ������ͷź����
  * @retval NO_KEY KEY1 KEY2
  */
uint8_t KEY_GetKey(void)
{
    if (KEY1_Val == 0)
    {
        Delay_ms(10);

        if (KEY1_Val == 0)
        {
            while (KEY1_Val == 0)
                ;

            return KEY1;
        }
    }
    else if (KEY2_Val == 0)
    {
        Delay_ms(10);

        if (KEY2_Val == 0)
        {
            while (KEY2_Val == 0)
                ;

            return KEY2;
        }
    }
    else
    {
        return NO_KEY;
    }

    return NO_KEY;
}

/**
  * @brief  �ȴ������²���ȡ��ֵ���ȴ������ͷź����
  * @retval KEY1 KEY2
  */
uint8_t KEY_GetKeyWait(void)
{
    uint8_t key = NO_KEY;

    while (key == NO_KEY)
    {
        key = KEY_GetKey();
    }

    return key;
}
