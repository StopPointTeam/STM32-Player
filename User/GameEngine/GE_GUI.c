//游戏引擎 GUI 库

#include "sys.h"
#include "key.h"
#include "lcd.h"

#include "GameEngine.h"

#include "GE_GUI.h"

/**
  * @brief  初始化 GE_GUI
  */
void GE_GUI_Init(void)
{
}

/**
  * @brief  绘制粗矩形
  * @param  x
  * @param  y
  * @param  width: 矩形的宽
  * @param  height: 矩形的高
  * @param  color: 矩形的颜色
  * @param  thickness: 线条厚度
  */
void GE_GUI_ThickRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint16_t thickness)
{
    for (uint16_t i = 0; i < thickness; i++)
        GE_Draw_Rectangle(x + i, y + i, width - 2 * i, height - 2 * i, color);
}

/**
  * @brief  文本框
  * @param  x
  * @param  y
  * @param  width: 矩形的宽
  * @param  height: 矩形的高
  * @param  font_color: 文字颜色
  * @param  fill_color: 填充颜色
  * @param  rectangle_color: 边框颜色
  * @param  thickness: 线条厚度
  * @param  str: 字符串
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
  * @brief  绘制双圈粗矩形
  * @param  x: 外圈坐标
  * @param  y: 外圈坐标
  * @param  width: 矩形的宽
  * @param  height: 矩形的高
  * @param  color: 矩形的颜色
  * @param  thickness: 线条厚度
  */
void GE_GUI_DoubleThickRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint16_t thickness)
{
    GE_GUI_ThickRectangle(x, y, width, height, color, thickness);
    GE_GUI_ThickRectangle(x + thickness + 1, y + thickness + 1, width - (thickness + 1) * 2, height - (thickness + 1) * 2, color, thickness);
}

/**
  * @brief  消息窗口。自动刷新屏幕
  * @param  x
  * @param  y
  * @param  width: 窗口的宽
  * @param  height: 窗口的高
  * @param  head: 标题
  * @param  content: 内容
  * @param  handler_func: 回调函数。例如，可以用延时实现延时退出，也可以用按键实现任意键退出。设置为 NULL 不执行回调，立即退出
  */
void GE_GUI_MsgBox(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *head, uint8_t *content, MsgBoxHandler handler_func)
{
    //边框
    GE_GUI_DoubleThickRectangle(x, y, width, height, BLACK, 2);

    //标题分隔线
    GE_Draw_Rectangle(x + 5, y + FONT_16 + 7, width - 10, 2, BLACK);

    //标题
    GE_Font_Print(x + 6, y + 6, BORDER_MAX, FONT_16, FONT_16, BLACK, WHITE, FALSE, head);

    //内容
    GE_Font_Print(x + 6, y + FONT_16 + 10, width - 12, BORDER_MAX, FONT_16, BLACK, WHITE, FALSE, content);

    GE_Draw_Disp();

    KEY_ClearKey();

    //调用回调函数
    if (handler_func != NULL)
        handler_func();
}

/**
  * @brief  菜单窗口。自动刷新屏幕
  * @param  x
  * @param  y
  * @param  width: 窗口的宽
  * @param  height: 窗口的高
  * @param  head: 标题
  * @param  content_amount: 菜单项数
  * @param  content: 菜单内容数组
  * @param  handler_func: 回调函数。设置为 NULL 不执行回调
  * @retval 被选中的选项序号，从 1 开始。返回 0 表示用户触发 JOY_L_DOWN 事件，此时不调用回调函数
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

    //边框
    GE_GUI_DoubleThickRectangle(x, y, width, height, BLACK, 2);

    //标题分隔线
    GE_Draw_Rectangle(x + 5, y + FONT_16 + 7, width - 10, 2, BLACK);

    //标题
    GE_Font_Print(x + 6, y + 6, BORDER_MAX, FONT_16, FONT_16, BLACK, WHITE, FALSE, head);

    //绘制选项
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

    //调用回调函数
    if (handler_func != NULL)
        handler_func(choice_num);

    return choice_num;
}
