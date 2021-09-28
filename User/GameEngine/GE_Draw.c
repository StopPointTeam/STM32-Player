//��Ϸ����ͼ����ƿ�

#include "stdlib.h"
#include "string.h"

#include "spi.h"

#include "lcd.h"
#include "GameEngine.h"

#include "GE_Draw.h"

/****************************************   �Դ�   ****************************************/

uint8_t GE_Draw_VRam[153600];

/*****************************************************************************************/

_ge_draw_pic_set ge_draw_pic_set;
_ge_draw_mono_set ge_draw_mono_set;

/**
  * @brief  ��ʼ�� GE_Draw
  */
void GE_Draw_Init(void)
{
    //����ͼƬ����
    ge_draw_pic_set.is_reverse = FALSE;
    ge_draw_pic_set.pos_mode = UP_LEFT;

    //���Ƶ�ɫͼ����
    ge_draw_mono_set.draw_mode = MONO_OR;
    ge_draw_mono_set.pos_mode = UP_LEFT;
    ge_draw_mono_set.mono_color = BLACK;
    ge_draw_mono_set.back_color = WHITE;

    GE_Draw_ClrAll(WHITE);
}

/**
  * @brief  ˢ����Ļ
  */
void GE_Draw_Disp(void)
{
    LCD_SetWin(0, 0, LCD_WIDTH, LCD_HEIGHT);
    LCD_SendCmdDataBytes(LCD_CMD_RAMWR, GE_Draw_VRam, 153600);
}

/**
  * @brief  ��ָ����ɫ����
  * @param  color: ��ɫ
  */
void GE_Draw_ClrAll(uint16_t color)
{
    GE_Draw_Fill(0, 0, LCD_WIDTH, LCD_HEIGHT, color);
}

/**
  * @brief  ��ָ����ɫ��������������ٶȽ���
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  width: ����Ŀ�
  * @param  height: ����ĸ�
  * @param  color: ��ɫ
  */
void GE_Draw_Fill(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color)
{
    uint32_t point_num = width * height;
    int16_t x0 = x, y0 = y;

    for (uint32_t i = 0; i < point_num; i++)
    {
        GE_Draw_Point(x++, y, color);

        if (x == x0 + width)
        {
            y++;
            x = x0;
        }
    }
}

/**
  * @brief  ��ָ�������ָ����ɫ�ĵ�
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  color: ��ɫ
  */
void GE_Draw_Point(int16_t x, int16_t y, uint16_t color)
{
    if (x < 0 || x > LCD_WIDTH - 1 || y < 0 || y > LCD_HEIGHT - 1)
        return;

    GE_Draw_VRam[(x + y * 320) * 2] = color >> 8;
    GE_Draw_VRam[(x + y * 320) * 2 + 1] = color;
}

/**
  * @brief  ��ȡָ������ĵ����ɫ�����겻�Ϸ����ذ�ɫ
  * @param  x: 0~319
  * @param  y: 0~239
  */
uint16_t GE_Draw_GetPoint(int16_t x, int16_t y)
{
    if (x < 0 || x > LCD_WIDTH - 1 || y < 0 || y > LCD_HEIGHT - 1)
        return WHITE;

    return ((uint16_t)GE_Draw_VRam[(x + y * 320) * 2] << 8) | GE_Draw_VRam[(x + y * 320) * 2 + 1];
}

/**
  * @brief  ���߶�����ָ��������
  * @param  x1: 0~319
  * @param  y1: 0~239
  * @param  x2: 0~319
  * @param  y3: 0~239
  * @param  color: ��ɫ
  */
void GE_Draw_Line(int16_t x0, int16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    //ʹ�� Bresenham �㷨
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
  * @brief  ��ָ����ɫ���ƾ��α߿�
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  width: ���εĿ�
  * @param  height: ���εĸ�
  * @param  color: ��ɫ
  */
void GE_Draw_Rectangle(int16_t x, int16_t y, uint16_t width, uint16_t height, uint16_t color)
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
  * @brief  ��ָ����ɫ����Բ
  * @param  xm: 0~319
  * @param  ym: 0~239
  * @param  r: Բ�İ뾶
  * @param  color: ��ɫ
  */
void GE_Draw_Circle(int16_t xm, int16_t ym, uint16_t r, uint16_t color)
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
  * @brief  ���ƾ���ͼƬ��ͼƬΪ const unsigned char ���飬4096 ɫ������ͼ��ͷ��
  *         ��ȷ��ͼƬ��ȫ����Ļ������
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  is_reverse: TRUE Ϊ��ɫ����ɫģʽ��ˢ�½���
  * @param  pos_mode: UP_LEFT Ϊ�����Ͻǿ�ʼ��ʾ��MID Ϊ������Ϊ������ʾ
  * @param  pic: ָ��ͼƬ
  * @param  width: ͼƬ�Ŀ�
  * @param  height: ͼƬ�ĸ�
  */
void GE_Draw_Pic(
    int16_t x,
    int16_t y,
    uint8_t is_reverse,
    uint8_t pos_mode,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height)
{
    if (pos_mode == MID)
    {
        x = x - width / 2;
        y = y - height / 2;
    }

    if (x == 0 && y == 0 && width == LCD_WIDTH && height == LCD_HEIGHT)
        memcpy(GE_Draw_VRam, pic, 153600);

    uint32_t point_num = width * height;
    int16_t x0 = x, y0 = y;
    uint16_t color;

    if (is_reverse == TRUE) //��ɫģʽ
    {
        for (uint32_t i = 0; i < point_num; i++)
        {
            color = 0xffff - (((uint16_t)pic[2 * i] << 8) | pic[2 * i + 1]);
            GE_Draw_Point(x++, y, color);

            if (x == x0 + width)
            {
                y++;
                x = x0;
            }
        }
    }
    else //�Ƿ�ɫģʽ
    {
        for (uint32_t i = 0; i < point_num; i++)
        {
            color = ((uint16_t)pic[2 * i] << 8) | pic[2 * i + 1];
            GE_Draw_Point(x++, y, color);

            if (x == x0 + width)
            {
                y++;
                x = x0;
            }
        }
    }
}

/**
  * @brief  ���ƾ���ͼƬ��ʹ������
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  pic: ָ��ͼƬ
  * @param  width: ͼƬ�Ŀ�
  * @param  height: ͼƬ�ĸ�
  */
void GE_Draw_Pic_WithSet(int16_t x, int16_t y, const unsigned char *pic, uint16_t width, uint16_t height)
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

/**
  * @brief  ���Ƶ�ɫλͼ��ͼƬΪ const unsigned char ���飬����ͼ��ͷ
  *         ��ȷ��ͼƬ��ȫ����Ļ������
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  draw_mode: ���Ʒ�ʽ��MONO_OR MONO_AND MONO_XOR MONO_COVER
  * @param  pos_mode: UP_LEFT Ϊ�����Ͻǿ�ʼ��ʾ��MID Ϊ������Ϊ������ʾ
  * @param  mono_color: ��ɫ
  * @param  back_color: ����ɫ��ֻ�� MONO_COVER ʱ��Ч
  * @param  pic: ָ��ͼƬ
  * @param  width: ͼƬ�Ŀ�
  * @param  height: ͼƬ�ĸ�
  */
void GE_Draw_Mono(
    int16_t x,
    int16_t y,
    uint8_t draw_mode,
    uint8_t pos_mode,
    uint16_t mono_color,
    uint16_t back_color,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height)
{
    if (pos_mode == MID)
    {
        x = x - width / 2;
        y = y - height / 2;
    }

    uint8_t temp8;
    int16_t x0 = x, y0 = y;

    //ѭ���������
    for (uint16_t i = 0; y - y0 < height; i++)
    {
        temp8 = pic[i];

        for (uint16_t j = 0; j < 8; j++)
        {
            if (temp8 & 0x80)
            {
                if (
                    (draw_mode == MONO_OR) ||
                    (draw_mode == MONO_AND && GE_Draw_GetPoint(x, y) == mono_color) ||
                    (draw_mode == MONO_XOR && GE_Draw_GetPoint(x, y) != mono_color) ||
                    (draw_mode == MONO_COVER))
                {
                    GE_Draw_Point(x, y, mono_color);
                }
            }
            else
            {
                if (
                    (draw_mode == MONO_AND && GE_Draw_GetPoint(x, y) != mono_color) ||
                    (draw_mode == MONO_XOR && GE_Draw_GetPoint(x, y) == mono_color) ||
                    (draw_mode == MONO_COVER))
                {
                    GE_Draw_Point(x, y, back_color);
                }
            }

            temp8 <<= 1;

            x++;
            if (x - x0 == width)
            {
                x = x0;
                y++;
                break;
            }
        }
    }
}

/**
  * @brief  ���Ƶ�ɫλͼ��ʹ�����á�ͼƬΪ const unsigned char ���飬����ͼ��ͷ
  *         ��ȷ��ͼƬ��ȫ����Ļ������
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  pic: ָ��ͼƬ
  * @param  width: ͼƬ�Ŀ�
  * @param  height: ͼƬ�ĸ�
  */
void GE_Draw_Mono_WithSet(
    int16_t x,
    int16_t y,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height)
{
    GE_Draw_Mono(
        x,
        y,
        ge_draw_mono_set.draw_mode,
        ge_draw_mono_set.pos_mode,
        ge_draw_mono_set.mono_color,
        ge_draw_mono_set.back_color,
        pic,
        width,
        height);
}
