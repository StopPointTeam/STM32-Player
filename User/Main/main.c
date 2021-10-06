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
#include "APP_Gobang.h"

int main(void)
{
    MPU_Config();       //���� MPU
    Cache_Enable();     //�� L1-Cache
    HAL_Init();         //��ʼ�� HAL ��
    SystemClock_Init(); //����ʱ��
    Uart_Init();        //��ʼ������
    SysTick_Init();     //���� SysTick �жϣ�����ʼ�������ʱ��
    ADC_Init();         //��ʼ�� ADC

    Uart_SetprintfCom(COM1);  //���� printf ����� COM1
    Uart_SetgetcharCom(COM1); //���� getchar �� COM1 ����

    //��ʼ������
    LED_Init();
    KEY_Init();
    LCD_Init();

    //��ʼ��ģ��
    GE_Init();
    SD_Init();

    HC25_Init();
    HC12_Init();

    Clock_Init();

    GE_Draw_ClrAll(WHITE);
    GE_Draw_Disp();

    printf("���ϵͳ��ʼ��\n");

    uint8_t content[7][GE_GUI_MENUBOX_CONTENT_LEN] = {"�Ķ���", "��Ƶ������", "�ɻ���ս", "����", "�ƶ���", "������", "����"};
    while (1)
    {
        GE_Draw_ClrAll(WHITE);
        switch (GE_GUI_MenuBox(5, 5, 310, 230, "STM32Player", 7, content, NULL))
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
            APP_Ball_Launcher();
        }
        break;

        case 6:
        {
            GE_Draw_ClrAll(WHITE);
            APP_Gobang_Launcher();
        }
        break;

        case 7:
        {
            GE_Draw_ClrAll(WHITE);
            APP_Setting_Launcher();
        }
        }
    }
}
