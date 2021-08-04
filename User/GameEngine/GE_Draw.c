#include "sys.h"

#include "lcd.h"
#include "GameEngine.h"

#include "GE_Draw.h"

/**************************************** ˽�к��� ****************************************/

void GE_Draw_SetWinAbs(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);
void GE_Draw_SetWin(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/*****************************************************************************************/

_ge_draw_set_private ge_draw_set_private;

_ge_draw_pic_set ge_draw_pic_set;

/**
  * @brief  ��ʼ�� GE_Draw
  */
void GE_Draw_Init(void)
{
    //˽�б���
    ge_draw_set_private.x_start = 0;
    ge_draw_set_private.y_start = 0;
    ge_draw_set_private.x_end = LCD_WIDTH - 1;
    ge_draw_set_private.y_end = LCD_HEIGHT - 1;

    //����ͼƬ����
    ge_draw_pic_set.is_reverse = FALSE;
    ge_draw_pic_set.pos_mode = UP_LEFT;
}

/**
  * @brief  ʹ�þ�������������ʾ���ڡ������ظ����ã���Ч��
  * @param  x_start: 0~319
  * @param  y_start: 0~239���������Ͻ�����
  * @param  x_end: 0~319
  * @param  y_end: 0~239���������½�����
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
  * @brief  ʹ�ô��ڴ�С������ʾ���ڡ������ظ����ã���Ч��
  * @param  x: 0~319
  * @param  y: 0~239��x y Ϊ�������Ͻ�����
  * @param  width: ���ڿ�ȡ������ 0
  * @param  height: ���ڸ߶ȡ������ 0
  */
void GE_Draw_SetWin(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    GE_Draw_SetWinAbs(x, y, x + width - 1, y + height - 1);
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
  * @brief  ��ָ�������ָ����ɫ�ĵ�
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  color: ��ɫ
  */
void GE_Draw_Point(uint16_t x, uint16_t y, uint16_t color)
{
    if (x < 0 || x > LCD_WIDTH - 1 || y < 0 || y > LCD_HEIGHT - 1)
        return;

    GE_Draw_SetWinAbs(x, y, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    LCD_SendCmdData16Bits(LCD_CMD_RAMWR, color);
}

/**
  * @brief  ���߶�����ָ��������
  * @param  x1: 0~319
  * @param  y1: 0~239
  * @param  x2: 0~319
  * @param  y3: 0~239
  * @param  color: ��ɫ
  */
void GE_Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
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
  * @brief  ��ָ����ɫ����Բ
  * @param  xm: 0~319
  * @param  ym: 0~239
  * @param  r: Բ�İ뾶
  * @param  color: ��ɫ
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
  * @brief  ���ƾ���ͼƬ��ͼƬΪ const unsigned char ���飬4096 ɫ������ͼ��ͷ
  * @param  x: 0~319
  * @param  y: 0~239
  * @param  is_reverse: TRUE Ϊ��ɫ����ɫģʽ��ˢ�½���
  * @param  pos_mode: UP_LEFT Ϊ�����Ͻǿ�ʼ��ʾ��MID Ϊ������Ϊ������ʾ
  * @param  pic: ָ��ͼƬ
  * @param  width: ͼƬ�Ŀ�
  * @param  height: ͼƬ�ĸ�
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

    if (is_reverse == TRUE) //��ɫģʽ
    {
        uint16_t data16_temp;
        uint32_t data8_count = width * height * 2;

        LCD_RAM_Wr;
        LCD_Data_Mode_On;

        for (uint32_t i = 0; i < data8_count; i += 2)
        {
            data16_temp = 0xffff - (((uint16_t)pic[i] << 8) | pic[i + 1]); //��ɫ
            SPI2_Write16Bits(data16_temp);
        }
    }
    else //�Ƿ�ɫģʽ
    {
        LCD_SendCmdDataBytes(LCD_CMD_RAMWR, (uint8_t *)pic, width * height * 2);
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
