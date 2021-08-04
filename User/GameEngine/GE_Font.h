//文字显示库

#ifndef __GE_FONT_H
#define __GE_FONT_H

void GE_FontInit(void);

/**************************************** 绘制汉字 ****************************************/
//字体大小
#define FONT_12 12
#define FONT_16 16
#define FONT_24 24
#define FONT_32 32
#define FONT_48 48
#define FONT_64 64

//边界范围
#define BORDER_MAX 0xffff

typedef struct
{
    uint8_t font_size;      //字体大小
    uint16_t font_color;    //字体颜色
    uint16_t back_color;    //背景颜色
    uint8_t is_transparent; //是否透明
} _ge_font_print_set;

extern _ge_font_print_set ge_font_print_set;

uint8_t GE_Font_Print(
    uint16_t x_start,
    uint16_t y_start,
    uint16_t width,
    uint16_t height,
    uint8_t font_size,
    uint16_t font_color,
    uint16_t back_color,
    uint8_t is_transparent,
    uint8_t *str);

uint8_t GE_Font_Print_WithSet(uint16_t x_start, uint16_t y_start, uint16_t width, uint16_t height, uint8_t *str);

/*****************************************************************************************/

#endif
