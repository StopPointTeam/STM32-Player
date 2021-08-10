//游戏引擎

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
    GE_Draw_Init();
    GE_Font_Init();
    GE_GUI_Init();
}
