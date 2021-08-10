#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "lcd.h"

#include "GameEngine.h"
#include "SD.h"

#include "APP_Reader.h"

int main(void)
{
    MPU_Config();             //配置 MPU
    Cache_Enable();           //打开 L1-Cache
    HAL_Init();               //初始化 HAL 库
    Clock_Init(160, 5, 2, 4); //设置时钟为 400MHz
    SYSCLK_Init(400);         //延时初始化

    //初始化外设
    LED_Init();
    KEY_Init();
    LCD_Init();

    //初始化模块
    GE_Init();

    if (SD_Init() == SD_OK)
    {
        APP_Reader_Launcher();
        SD_DeInit();

        GE_Draw_ClrAll(WHITE);
        GE_Draw_Pic_WithSet(0, 0, BORDER_MAX, BORDER_MAX, "阅读器运行结束，请重置！");
    }

    while (1)
    {
    }
}
