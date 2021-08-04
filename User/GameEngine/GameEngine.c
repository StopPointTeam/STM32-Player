#include "sys.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"

#include "GameEngine.h"

/**
  * @brief  初始化游戏引擎。请首先初始化硬件
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
