//��Ϸ���� GUI ��

#include "sys.h"
#include "key.h"
#include "lcd.h"

#include "GameEngine.h"

#include "GE_GUI.h"

/**
  * @brief  ��ʼ�� GE_GUI
  */
void GE_GUI_Init(void)
{
}

/**
  * @brief  ���ƴ־���
  * @param  x
  * @param  y
  * @param  width: ���εĿ�
  * @param  height: ���εĸ�
  * @param  color: ���ε���ɫ
  * @param  thickness: �������
  */
void GE_GUI_ThickRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint16_t thickness)
{
    for (uint16_t i = 0; i < thickness; i++)
        GE_Draw_Rectangle(x + i, y + i, width - 2 * i, height - 2 * i, color);
}

/**
  * @brief  �ı���
  * @param  x
  * @param  y
  * @param  width: ���εĿ�
  * @param  height: ���εĸ�
  * @param  font_color: ������ɫ
  * @param  fill_color: �����ɫ
  * @param  rectangle_color: �߿���ɫ
  * @param  thickness: �������
  * @param  str: �ַ���
  */
void GE_GUI_TextBox(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    uint16_t font_color,
    uint16_t fill_color,
    uint16_t rectangle_color,
    uint16_t thickness,
    uint8_t *str)
{
    GE_GUI_ThickRectangle(x, y, width, height, rectangle_color, thickness);
    GE_Draw_Fill(x + thickness, y + thickness, width - 2 * thickness, height - 2 * thickness, fill_color);
    GE_Font_Print(x + thickness + 1, y + thickness + 1, width - 2 * (thickness + 1), height - 2 * (thickness + 1), FONT_16, font_color, WHITE, TRUE, str);
}

/**
  * @brief  ����˫Ȧ�־���
  * @param  x: ��Ȧ����
  * @param  y: ��Ȧ����
  * @param  width: ���εĿ�
  * @param  height: ���εĸ�
  * @param  color: ���ε���ɫ
  * @param  thickness: �������
  */
void GE_GUI_DoubleThickRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint16_t thickness)
{
    GE_GUI_ThickRectangle(x, y, width, height, color, thickness);
    GE_GUI_ThickRectangle(x + thickness + 1, y + thickness + 1, width - (thickness + 1) * 2, height - (thickness + 1) * 2, color, thickness);
}

/**
  * @brief  ��Ϣ���ڡ��Զ�ˢ����Ļ
  * @param  x
  * @param  y
  * @param  width: ���ڵĿ�
  * @param  height: ���ڵĸ�
  * @param  head: ����
  * @param  content: ����
  * @param  handler_func: �ص����������磬��������ʱʵ����ʱ�˳���Ҳ�����ð���ʵ��������˳�������Ϊ NULL ��ִ�лص��������˳�
  */
void GE_GUI_MsgBox(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *head, uint8_t *content, MsgBoxHandler handler_func)
{
    //�߿�
    GE_GUI_DoubleThickRectangle(x, y, width, height, BLACK, 2);

    //����ָ���
    GE_Draw_Rectangle(x + 5, y + FONT_16 + 7, width - 10, 2, BLACK);

    //����
    GE_Font_Print(x + 6, y + 6, BORDER_MAX, FONT_16, FONT_16, BLACK, WHITE, FALSE, head);

    //����
    GE_Font_Print(x + 6, y + FONT_16 + 10, width - 12, BORDER_MAX, FONT_16, BLACK, WHITE, FALSE, content);

    GE_Draw_Disp();

    KEY_ClearKey();

    //���ûص�����
    if (handler_func != NULL)
        handler_func();
}

/**
  * @brief  �˵����ڡ��Զ�ˢ����Ļ
  * @param  x
  * @param  y
  * @param  width: ���ڵĿ�
  * @param  height: ���ڵĸ�
  * @param  head: ����
  * @param  content_amount: �˵�����
  * @param  content: �˵���������
  * @param  handler_func: �ص�����������Ϊ NULL ��ִ�лص�
  * @retval ��ѡ�е�ѡ����ţ��� 1 ��ʼ������ 0 ��ʾ�û����� JOY_L_DOWN �¼�����ʱ�����ûص�����
  */
uint8_t GE_GUI_MenuBox(
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height,
    uint8_t *head,
    uint8_t content_amount,
    uint8_t content[][GE_GUI_MENUBOX_CONTENT_LEN],
    MenuBoxHandler handler_func)
{
    uint8_t choice_num = 1;

    //�߿�
    GE_GUI_DoubleThickRectangle(x, y, width, height, BLACK, 2);

    //����ָ���
    GE_Draw_Rectangle(x + 5, y + FONT_16 + 7, width - 10, 2, BLACK);

    //����
    GE_Font_Print(x + 6, y + 6, BORDER_MAX, FONT_16, FONT_16, BLACK, WHITE, FALSE, head);

    //����ѡ��
    for (uint8_t i = 0; i < content_amount; i++)
    {
        if (i + 1 == choice_num)
            GE_GUI_TextBox(x + 4, y + FONT_16 + 8 + i * (FONT_16 + 3), width - 8, FONT_16 + 4, WHITE, BLUE, BLACK, 1, content[i]);
        else
            GE_GUI_TextBox(x + 4, y + FONT_16 + 8 + i * (FONT_16 + 3), width - 8, FONT_16 + 4, BLACK, WHITE, BLACK, 1, content[i]);
    }

    GE_Draw_Disp();

    KEY_ClearKey();

    while (1)
    {
        uint8_t key = KEY_GetKeyWait();

        if (key == JOY_OK_UP)
        {
            break;
        }
        else if (key == JOY_L_DOWN)
        {
            return 0;
        }
        else if (key == JOY_U_DOWN && choice_num > 1)
        {
            GE_GUI_TextBox(x + 4, y + FONT_16 + 8 + (choice_num - 1) * (FONT_16 + 3), width - 8, FONT_16 + 4, BLACK, WHITE, BLACK, 1, content[choice_num - 1]);
            choice_num--;
            GE_GUI_TextBox(x + 4, y + FONT_16 + 8 + (choice_num - 1) * (FONT_16 + 3), width - 8, FONT_16 + 4, WHITE, BLUE, BLACK, 1, content[choice_num - 1]);

            GE_Draw_Disp();
        }
        else if (key == JOY_D_DOWN && choice_num < content_amount)
        {
            GE_GUI_TextBox(x + 4, y + FONT_16 + 8 + (choice_num - 1) * (FONT_16 + 3), width - 8, FONT_16 + 4, BLACK, WHITE, BLACK, 1, content[choice_num - 1]);
            choice_num++;
            GE_GUI_TextBox(x + 4, y + FONT_16 + 8 + (choice_num - 1) * (FONT_16 + 3), width - 8, FONT_16 + 4, WHITE, BLUE, BLACK, 1, content[choice_num - 1]);

            GE_Draw_Disp();
        }
    }

    //���ûص�����
    if (handler_func != NULL)
        handler_func(choice_num);

    return choice_num;
}
