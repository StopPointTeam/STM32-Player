#include "sys.h"

#include "lcd.h"
#include "GameEngine.h"

#include "GE_Draw.h"

/**************************************** 私有函数 ****************************************/

void GE_Draw_SetWinAbs(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
void GE_Draw_SetWin(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/*****************************************************************************************/

_ge_draw_set_private ge_draw_set_private;

_ge_draw_pic_set ge_draw_pic_set;

/**
  * @brief  初始化 GE_Draw
  */
void GE_Draw_Init(void)
{
    //私有变量
    ge_draw_set_private.x_start = 0;
    ge_draw_set_private.y_start = 0;
    ge_draw_set_private.x_end = LCD_WIDTH - 1;
    ge_draw_set_private.y_end = LCD_HEIGHT - 1;

    //绘制图片设置
    ge_draw_pic_set.is_reverse = FALSE;
    ge_draw_pic_set.pos_mode = UP_LEFT;
}

/**
  * @brief  使用绝对坐标设置显示窗口。忽略重复设置，高效率
  * @param  x_start: 0~319
  * @param  y_start: 0~239。窗口左上角坐标
  * @param  x_end: 0~319
  * @param  y_end: 0~239。窗口右下角坐标
  */
void GE_Draw_SetWinAbs(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    if (x_end == ge_draw_set_private.x_end && y_end == ge_draw_set_private.y_end)
    {
        if (x_start == ge_draw_set_private.x_start && y_start == ge_draw_set_private.y_start)
        {
            return;
        }
        else
        {
            ge_draw_set_private.x_start = x_start;
            ge_draw_set_private.y_start = y_start;

            LCD_SendCmdData16Bits(LCD_CMD_CASET, x_start);
            LCD_SendCmdData16Bits(LCD_CMD_PASET, y_start);
        }
    }
    else
    {
        ge_draw_set_private.x_start = x_start;
        ge_draw_set_private.y_start = y_start;
        ge_draw_set_private.x_end = x_end;
        ge_draw_set_private.y_end = y_end;

        LCD_SendCmdData32Bits(LCD_CMD_CASET, (x_start << 16) | x_end);
        LCD_SendCmdData32Bits(LCD_CMD_PASET, (y_start << 16) | y_end);
    }
}

/**
  * @brief  使用窗口大小设置显示窗口。忽略重复设置，高效率
  * @param  x: 0~319
  * @param  y: 0~239。x y 为窗口左上角坐标
  * @param  width: 窗口宽度。需大于 0
  * @param  height: 窗口高度。需大于 0
  */
void GE_Draw_SetWin(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    GE_Draw_SetWinAbs(x, y, x + width - 1, y + height - 1);
}

/**
  * @brief  以指定颜色清屏
  * @param  color: 颜色
  */
void GE_Draw_ClrAll(uint16_t color)
{
    GE_Draw_Fill(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

/**
  * @brief  以指定颜色填充矩形区域。填充速度较慢
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  width: 区域的宽
  * @param  height: 区域的高
  * @param  color: 颜色
  */
void GE_Draw_Fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    GE_Draw_SetWin(x, y, width, height);

    uint32_t data16_count = width * height;

    LCD_RAM_Wr;
    LCD_Data_Mode_On;

    for (uint32_t i = 0; i < data16_count; i++)
        SPI2_Write16Bits(color);
}

/**
  * @brief  在指定坐标绘指定颜色的点
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  color: 颜色
  */
void GE_Draw_Point(uint16_t x, uint16_t y, uint16_t color)
{
    if (x < 0 || x > LCD_WIDTH - 1 || y < 0 || y > LCD_HEIGHT - 1)
        return;

    GE_Draw_SetWinAbs(x, y, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    LCD_SendCmdData16Bits(LCD_CMD_RAMWR, color);
}

/**
  * @brief  用线段连接指定两坐标
  * @param  x1: 0~319
  * @param  y1: 0~239
  * @param  x2: 0~319
  * @param  y3: 0~239
  * @param  color: 颜色
  */
void GE_Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    //使用 Bresenham 算法
    int16_t dx = abs(x1 - x0);
    int8_t sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0);
    int8_t sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy, e2;

    while (1)
    {
        GE_Draw_Point(x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

/**
  * @brief  以指定颜色绘制矩形边框
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  width: 矩形的宽
  * @param  height: 矩形的高
  * @param  color: 颜色
  */
void GE_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    uint16_t i;

    for (i = 0; i < width; i++)
    {
        GE_Draw_Point(x + i, y, color);
        GE_Draw_Point(x + i, y + height - 1, color);
    }

    for (i = 0; i < height; i++)
    {
        GE_Draw_Point(x, y + i, color);
        GE_Draw_Point(x + width - 1, y + i, color);
    }
}

/**
  * @brief  以指定颜色绘制圆
  * @param  xm: 0~319
  * @param  ym: 0~239
  * @param  r: 圆的半径
  * @param  color: 颜色
  */
void GE_Draw_Circle(uint16_t xm, uint16_t ym, uint16_t r, uint16_t color)
{
    int16_t x = -r, y = 0, err = 2 - 2 * r, rm = r;

    do
    {
        GE_Draw_Point(xm - x, ym + y, color);
        GE_Draw_Point(xm - y, ym - x, color);
        GE_Draw_Point(xm + x, ym - y, color);
        GE_Draw_Point(xm + y, ym + x, color);
        rm = err;
        if (rm > x)
            err += ++x * 2 + 1;
        if (rm <= y)
            err += ++y * 2 + 1;
    } while (x < 0);
}

/**
  * @brief  绘制矩形图片。图片为 const unsigned char 数组，4096 色，不含图像头
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  is_reverse: TRUE 为反色。反色模式下刷新较慢
  * @param  pos_mode: UP_LEFT 为从左上角开始显示，MID 为以坐标为中心显示
  * @param  pic: 指向图片
  * @param  width: 图片的宽
  * @param  height: 图片的高
  */
void GE_Draw_Pic(
    uint16_t x,
    uint16_t y,
    uint8_t is_reverse,
    uint8_t pos_mode,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height)
{
    if (pos_mode == UP_LEFT)
        GE_Draw_SetWin(x, y, width, height);
    else
        GE_Draw_SetWin(x - width / 2, y - height / 2, width, height);

    if (is_reverse == TRUE) //反色模式
    {
        uint16_t data16_temp;
        uint32_t data8_count = width * height * 2;

        LCD_RAM_Wr;
        LCD_Data_Mode_On;

        for (uint32_t i = 0; i < data8_count; i += 2)
        {
            data16_temp = 0xffff - (((uint16_t)pic[i] << 8) | pic[i + 1]); //反色
            SPI2_Write16Bits(data16_temp);
        }
    }
    else //非反色模式
    {
        LCD_SendCmdDataBytes(LCD_CMD_RAMWR, (uint8_t *)pic, width * height * 2);
    }
}

/**
  * @brief  绘制矩形图片，使用设置
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  pic: 指向图片
  * @param  width: 图片的宽
  * @param  height: 图片的高
  */
void GE_Draw_Pic_WithSet(uint16_t x, uint16_t y, const unsigned char *pic, uint16_t width, uint16_t height)
{
    GE_Draw_Pic(
        x,
        y,
        ge_draw_pic_set.is_reverse,
        ge_draw_pic_set.pos_mode,
        pic,
        width,
        height);
}
