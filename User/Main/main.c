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
    MPU_Config();             //���� MPU
    Cache_Enable();           //�� L1-Cache
    HAL_Init();               //��ʼ�� HAL ��
    Clock_Init(160, 5, 2, 4); //����ʱ��Ϊ 400MHz
    SYSCLK_Init(400);         //��ʱ��ʼ��

    //��ʼ������
    LED_Init();
    KEY_Init();
    LCD_Init();

    //��ʼ��ģ��
    GE_Init();

    if (SD_Init() == SD_OK)
    {
        APP_Reader_Launcher();
        SD_DeInit();

        GE_Draw_ClrAll(WHITE);
        GE_Draw_Pic_WithSet(0, 0, BORDER_MAX, BORDER_MAX, "�Ķ������н����������ã�");
    }

    while (1)
    {
    }
}
