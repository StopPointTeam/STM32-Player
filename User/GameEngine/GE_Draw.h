#ifndef __GE_DRAW_H
#define __GE_DRAW_H

//颜色
#define BLACK 0x0000
#define NAVY 0x000f
#define DARK_GREEN 0x03e0
#define DARK_CYAN 0x03ef
#define MAROON 0x7800
#define PURPLE 0x780f
#define OLIVE 0x7be0
#define LIGHT_GREY 0xc618
#define DARK_GREY 0x7bef
#define BLUE 0x001f
#define GREEN 0x07e0
#define CYAN 0x07ff
#define RED 0xf800
#define MAGENTA 0xf81f
#define YELLOW 0xffe0
#define WHITE 0xffff

/**************************************** 私有变量 ****************************************/
typedef struct
{
    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;
} _ge_draw_set_private;

/*****************************************************************************************/

/***************************************** 初始化 *****************************************/

void GE_Draw_Init(void);

/*****************************************************************************************/

/**************************************** 清空屏幕 ****************************************/

void GE_Draw_ClrAll(uint16_t color);

/*****************************************************************************************/

/**************************************** 绘图函数 ****************************************/

void GE_Draw_Fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void GE_Draw_Point(uint16_t x, uint16_t y, uint16_t color);
void GE_Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void GE_Draw_Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void GE_Draw_Circle(uint16_t xm, uint16_t ym, uint16_t r, uint16_t color);

/**************************************** 绘制图片 ****************************************/
//显示位置
#define UP_LEFT 0 //左上角
#define MID 1     //中心

typedef struct
{
    uint8_t is_reverse; //是否反色
    uint8_t pos_mode;   //显示位置
} _ge_draw_pic_set;

extern _ge_draw_pic_set ge_draw_pic_set;

void GE_Draw_Pic(
    uint16_t x,
    uint16_t y,
    uint8_t is_reverse,
    uint8_t pos_mode,
    const unsigned char *pic,
    uint16_t width,
    uint16_t height);
void GE_Draw_Pic_WithSet(uint16_t x, uint16_t y, const unsigned char *pic, uint16_t width, uint16_t height);

/*****************************************************************************************/

#endif
