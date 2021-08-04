#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "lcd.h"

#include "GameEngine.h"

#include "Picture.h"

int main(void)
{
    Cache_Enable();           //�� L1-Cache
    HAL_Init();               //��ʼ�� HAL ��
    Clock_Init(160, 5, 2, 4); //����ʱ��Ϊ 400MHz
    SYSCLK_Init(400);         //��ʱ��ʼ��

    LED_Init();
    KEY_Init();
    LCD_Init();

    GE_Init();

    //LED KEY LCD ���ܲ���
    GE_Draw_ClrAll(WHITE);

    //������ʾ����
    ge_font_print_set.font_size = FONT_16;

    GE_Font_Print_WithSet(0, 0, BORDER_MAX, BORDER_MAX, "1. ͼ�λ��Ʋ���");
    GE_Font_Print_WithSet(0, LCD_HEIGHT - FONT_16, BORDER_MAX, BORDER_MAX, "�����������");

    GE_Draw_Point(160, 120, RED);
    GE_Draw_Line(0, 0, 320, 240, GREEN);
    GE_Draw_Fill(100, 100, 40, 40, BLUE);
    GE_Draw_Rectangle(200, 200, 20, 20, YELLOW);
    GE_Draw_Circle(160, 120, 90, PURPLE);

    KEY_GetKeyWait();

    GE_Draw_ClrAll(WHITE);
    GE_Font_Print_WithSet(0, 0, BORDER_MAX, BORDER_MAX, "2. ������ʾ����");
    GE_Font_Print_WithSet(0, LCD_HEIGHT - FONT_16, BORDER_MAX, BORDER_MAX, "�����������");

    GE_Font_Print(
        0,
        FONT_16,
        BORDER_MAX,
        BORDER_MAX,
        FONT_32,
        BLACK,
        WHITE,
        TRUE,
        "Look������һ�����Զ����еľ��ӡ�");

    GE_Font_Print(
        0,
        FONT_16 + FONT_32 * 2,
        BORDER_MAX,
        BORDER_MAX,
        FONT_24,
        BLACK,
        WHITE,
        TRUE,
        "Look������һ����\n���еľ��ӡ�");

    GE_Font_Print(
        0,
        FONT_16 + FONT_32 * 2 + FONT_24 * 2,
        BORDER_MAX,
        BORDER_MAX,
        FONT_12,
        BLACK,
        WHITE,
        TRUE,
        "Look������һ����\n���еľ��ӡ�");

    KEY_GetKeyWait();

    GE_Draw_ClrAll(WHITE);
    GE_Font_Print_WithSet(0, 0, BORDER_MAX, BORDER_MAX, "3. ͼ����ʾ����");
    GE_Font_Print_WithSet(0, LCD_HEIGHT - FONT_16, BORDER_MAX, BORDER_MAX, "�����������");

    KEY_GetKeyWait();

    GE_Draw_Pic(0, 0, FALSE, UP_LEFT, pic_minecraft, 320, 240);

    KEY_GetKeyWait();

    GE_Draw_ClrAll(WHITE);
    GE_Font_Print_WithSet(0, 0, BORDER_MAX, BORDER_MAX, "4. LED ����");
    GE_Font_Print_WithSet(0, LCD_HEIGHT - FONT_16, BORDER_MAX, BORDER_MAX, "���������LED ��ת");

    while (1)
    {
        KEY_GetKeyWait();
        LED2_Toggle;
    }
}
