#include "stdio.h"

#include "sys.h"
#include "systick.h"

#include "led.h"
#include "key.h"
#include "lcd.h"
#include "uart.h"
#include "adc.h"
#include "hc25.h"
#include "hc12.h"

#include "GameEngine.h"
#include "SD.h"
#include "WLAN.h"
#include "Clock.h"

#include "APP_Reader.h"
#include "APP_Video.h"
#include "APP_Plane.h"
#include "APP_Setting.h"
#include "APP_Weather.h"
#include "APP_Ball.h"

int main(void)
{
    MPU_Config();       //配置 MPU
    Cache_Enable();     //打开 L1-Cache
    HAL_Init();         //初始化 HAL 库
    SystemClock_Init(); //设置时钟
    Uart_Init();        //初始化串口
    SysTick_Init();     //配置 SysTick 中断，并初始化软件定时器
    ADC_Init();         //初始化 ADC

    Uart_SetprintfCom(COM1);  //设置 printf 输出到 COM1
    Uart_SetgetcharCom(COM1); //设置 getchar 从 COM1 输入

    //初始化外设
    LED_Init();
    KEY_Init();
    LCD_Init();

    //初始化模块
    GE_Init();

    GE_Draw_ClrAll(WHITE);
    GE_Draw_Disp();

    SD_Init();

    printf("完成系统初始化\n");

    HC25_Init();
    HC12_Init();

    APP_Ball_Launcher();
    while (1);

    Clock_Init();

    uint8_t content[5][GE_GUI_MENUBOX_CONTENT_LEN] = {"阅读器", "视频播放器", "飞机大战", "天气", "设置"};
    while (1)
    {
        GE_Draw_ClrAll(WHITE);
        switch (GE_GUI_MenuBox(5, 5, 310, 230, "STM32Player", 5, content, NULL))
        {
        case 1:
        {
            GE_Draw_ClrAll(WHITE);
            APP_Reader_Launcher();
        }
        break;

        case 2:
        {
            GE_Draw_ClrAll(WHITE);
            APP_Video_Launcher();
        }
        break;

        case 3:
        {
            GE_Draw_ClrAll(WHITE);
            APP_Plane_Launcher();
        }
        break;

        case 4:
        {
            GE_Draw_ClrAll(WHITE);
            APP_Weather_Launcher();
        }
        break;

        case 5:
        {
            GE_Draw_ClrAll(WHITE);
            APP_Setting_Launcher();
        }
        }
    }
}
