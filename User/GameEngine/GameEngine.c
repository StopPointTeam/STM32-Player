//��Ϸ����

#include "sys.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"

#include "GameEngine.h"

/**
  * @brief  ��ʼ����Ϸ���档�����ȳ�ʼ��Ӳ��
  */
void GE_Init(void)
{
    GE_Draw_Init();
    GE_Font_Init();
    GE_GUI_Init();
}
