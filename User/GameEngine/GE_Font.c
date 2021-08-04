//������ʾ��

#include "sys.h"
#include "lcd.h"
#include "GameEngine.h"

#include "GE_Font.h"

#define GE_Font_GetACSIIDataSize(__FONT_SIZE__) ((__FONT_SIZE__ / 8 + ((__FONT_SIZE__ % 8) ? 1 : 0)) * (__FONT_SIZE__ / 2))
#define GE_Font_GetGBKDataSize(__FONT_SIZE__) ((__FONT_SIZE__ / 8 + ((__FONT_SIZE__ % 8) ? 1 : 0)) * (__FONT_SIZE__))

/**************************************** ˽�к��� ****************************************/

uint8_t GE_Font_GetACSII(uint8_t *font, uint8_t *ch, uint8_t font_size);
uint8_t GE_Font_PrintACSII(uint16_t x, uint16_t y, uint8_t *ch, uint8_t font_size, uint16_t font_color, uint16_t back_color, uint8_t is_transparent);

uint8_t GE_Font_GetGBK(uint8_t *font, uint8_t *ch, uint8_t font_size);
uint8_t GE_Font_PrintGBK(uint16_t x, uint16_t y, uint8_t *ch, uint8_t font_size, uint16_t font_color, uint16_t back_color, uint8_t is_transparent);

/*****************************************************************************************/

_ge_font_print_set ge_font_print_set;

/**
  * @brief  ��ʼ�� GE_Font
  */
void GE_Font_Init(void)
{
    //���ƺ�������
    ge_font_print_set.font_size = FONT_12;
    ge_font_print_set.font_color = BLACK;
    ge_font_print_set.back_color = WHITE;
    ge_font_print_set.is_transparent = TRUE;
}

/**
  * @brief  ���ֿ�� ACSII �ַ���˵����
  */
uint8_t GE_Font_GetACSII(uint8_t *font, uint8_t *ch, uint8_t font_size)
{
    uint32_t addr, offset;
    uint16_t data_size = GE_Font_GetACSIIDataSize(font_size);

    offset = (uint32_t)data_size * (*ch);

    switch (font_size)
    {
    case FONT_12:
        addr = offset + ADDR_ASCII12;
        break;
    case FONT_16:
        addr = offset + ADDR_ASCII16;
        break;
    case FONT_24:
        addr = offset + ADDR_ASCII24;
        break;
    case FONT_32:
        addr = offset + ADDR_ASCII32;
        break;
    case FONT_48:
        addr = offset + ADDR_ASCII48;
        break;
    case FONT_64:
        addr = offset + ADDR_ASCII64;
        break;

    default:
        return 1;
    }

    LCD_Font_ReadAddr(font, addr, data_size); //���ֿ��ȡ�ַ��� font

    return 0;
}

/**
  * @brief  ��ʾ ACSII �ַ���˵����
  */
uint8_t GE_Font_PrintACSII(uint16_t x, uint16_t y, uint8_t *ch, uint8_t font_size, uint16_t font_color, uint16_t back_color, uint8_t is_transparent)
{
    uint16_t data_size = GE_Font_GetACSIIDataSize(font_size);

    //���ֿ��ȡ�ַ�
    uint8_t font[256]; //ʹ�� FONT_64 ʱ��Ҫ 256 ��С
    if (GE_Font_GetACSII(font, ch, font_size) != 0)
        return 1;

    uint8_t temp8;
    uint16_t x0 = x, y0 = y;

    //ѭ���������
    for (uint16_t i = 0; i < data_size; i++)
    {
        temp8 = font[i];

        for (uint16_t j = 0; j < 8; j++)
        {
            if (temp8 & 0x80)
                GE_Draw_Point(x, y, font_color);
            else if (is_transparent == FALSE) //��͸��ʱʹ�ñ���ɫ���
                GE_Draw_Point(x, y, back_color);

            temp8 <<= 1;

            y++;
            if ((y - y0) == font_size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }

    return 0;
}

/**
  * @brief  ���ֿ�� GBK �ַ���˵����
  */
uint8_t GE_Font_GetGBK(uint8_t *font, uint8_t *ch, uint8_t font_size)
{
    if (font_size >= FONT_48)
        return 1;

    uint32_t addr, offset;
    uint16_t data_size = GE_Font_GetGBKDataSize(font_size);

    uint8_t qh = *ch;
    uint8_t ql = *(++ch);

    if (qh < 0x81 || ql < 0x40 || ql == 0xff || qh == 0xff) //�Ƿ�����
        return 1;

    if (ql < 0x7f)
        ql -= 0x40;
    else
        ql -= 0x41;

    qh -= 0x81;

    offset = (uint32_t)data_size * (190 * (uint32_t)qh + ql);

    switch (font_size)
    {
    case FONT_12:
        addr = offset + ADDR_GBK12;
        break;
    case FONT_16:
        addr = offset + ADDR_GBK16;
        break;
    case FONT_24:
        addr = offset + ADDR_GBK24;
        break;
    case FONT_32:
        addr = offset + ADDR_GBK32;
        break;

    default:
        return 1;
    }

    LCD_Font_ReadAddr(font, addr, data_size); //���ֿ��ȡ�ַ��� font

    return 0;
}

/**
  * @brief  ��ʾ GBK �ַ���˵����
  */
uint8_t GE_Font_PrintGBK(uint16_t x, uint16_t y, uint8_t *ch, uint8_t font_size, uint16_t font_color, uint16_t back_color, uint8_t is_transparent)
{
    uint16_t data_size = GE_Font_GetGBKDataSize(font_size);

    //���ֿ��ȡ�ַ�
    uint8_t font[128]; //ʹ�� FONT_32 ʱ��Ҫ 128 ��С
    if (GE_Font_GetGBK(font, ch, font_size) != 0)
        return 1;

    uint8_t temp8;
    uint16_t x0 = x, y0 = y;

    //ѭ���������
    for (uint16_t i = 0; i < data_size; i++)
    {
        temp8 = font[i];

        for (uint16_t j = 0; j < 8; j++)
        {
            if (temp8 & 0x80)
                GE_Draw_Point(x, y, font_color);
            else if (is_transparent == FALSE) //��͸��ʱʹ�ñ���ɫ���
                GE_Draw_Point(x, y, back_color);

            temp8 <<= 1;

            y++;
            if ((y - y0) == font_size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }

    return 0;
}

/**
  * @brief  ��ָ����������ʾ�ַ�����֧�� ASCII��GBK ����
  * @param  x_start
  * @param  y_start
  * @param  width: ��ʾ���Ŀ�
  * @param  height: ��ʾ���ڵĸ�
  * @param  font_size: �����С��ע�⣺�������֣�ֻ��ʹ�� FONT_12 24 32 48
  * @param  font_color: ������ɫ
  * @param  back_color: ������ɫ��͸��ʱ��Ч
  * @param  is_transparent: �Ƿ�͸��
  * @param  str: �ַ�������Ϊ uint8_t *
  */
uint8_t GE_Font_Print(
    uint16_t x_start,
    uint16_t y_start,
    uint16_t width,
    uint16_t height,
    uint8_t font_size,
    uint16_t font_color,
    uint16_t back_color,
    uint8_t is_transparent,
    uint8_t *str)
{
    uint16_t x_end_plus_1;
    uint16_t y_end_plus_1;
    uint16_t x = x_start;
    uint16_t y = y_start;

    if (width == BORDER_MAX)
        x_end_plus_1 = LCD_WIDTH;
    else
        x_end_plus_1 = x_start + width;

    if (height == BORDER_MAX)
        y_end_plus_1 = LCD_HEIGHT;
    else
        y_end_plus_1 = y_start + height;

    uint8_t is_print_all = FALSE;

    while (1)
    {
        if (*(str) <= 0x80) //ASCII �ַ�
        {
            if (*str == '\0')
            {
                is_print_all = TRUE;
                break;
            }

            if ((x + font_size / 2 > x_end_plus_1) || (*str == '\n'))
            {
                x = x_start;
                y += font_size;

                if (y + font_size > y_end_plus_1)
                    break;
            }

            if (*str != '\n')
            {
                GE_Font_PrintACSII(x, y, str, font_size, font_color, back_color, is_transparent);
                x += font_size / 2;
            }

            str++;
        }
        else if (*(str) > 0x80) //GBK �ַ�
        {
            if (x + font_size > x_end_plus_1)
            {
                x = x_start;
                y += font_size;

                if (y + font_size > y_end_plus_1)
                    break;
            }

            GE_Font_PrintGBK(x, y, str, font_size, font_color, back_color, is_transparent);
            x += font_size;
            str++;
            str++;
        }
    }

    return is_print_all;
}

/**
  * @brief  ��ָ����������ʾ�ַ�����ʹ�����á�֧�� ASCII��GBK ����
  * @param  x_start
  * @param  y_start
  * @param  width: ��ʾ���Ŀ�
  * @param  height: ��ʾ���ڵĸ�
  * @param  str: �ַ�������Ϊ uint8_t *
  */
uint8_t GE_Font_Print_WithSet(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, uint8_t *str)
{
    return GE_Font_Print(
        x_start,
        y_start,
        width,
        height,
        ge_font_print_set.font_size,
        ge_font_print_set.font_color,
        ge_font_print_set.back_color,
        ge_font_print_set.is_transparent,
        str);
}
