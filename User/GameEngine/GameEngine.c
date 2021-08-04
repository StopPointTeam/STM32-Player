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
#ifdef EN_GE_DRAW
    GE_Draw_Init();
#endif

#ifdef EN_GE_FONT
    GE_Font_Init();
#endif
}
